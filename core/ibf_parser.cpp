/***************************************************************************
 *   Copyright (C) 2018 by Oleksii S. Malakhov <brezerk@gmail.com>         *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "ibf_parser.hpp"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8


ibf_parserer::ibf_parserer(const std::string& name) {
    this->data_file_ = name;
}

ibf_parserer::~ibf_parserer() {

}

bool ibf_parserer::load() {
    if (!boost::filesystem::exists(this->data_file_))
    {
        std::cerr << "Can't find file by path: " << this->data_file_ << std::endl;
        return false;
    }
    return true;
}

bool ibf_parserer::parse(int action, std::string param) {
    std::ifstream file;
    try {
        file.open(this->data_file_, std::ifstream::in);
    } catch (std::ifstream::failure e) {
        std::cerr << "Exception opening file: " << e.what() << std::endl;
        return false;
    }

    std::string str;

    //list of match guids
    std::set<std::string> node_guids;
    std::pair<std::set<std::string>::iterator,bool> node_guids_ret;

    //peer and neigbors store
    uint32_t curr_port=0;
    uint32_t max_ports=0;

    const boost::regex getNodesRegEx("^{(.*)} {(.*)} (.*)$");
    boost::smatch getNodesRegExResults;

    const boost::regex linkStateEx("^PHY=(\\w+) LOG=(\\w+) SPD=(\\d+)$");
    boost::smatch linkStateExResults;

    while (std::getline(file, str))
    {
        boost::regex_match(str, getNodesRegExResults, getNodesRegEx);

#ifdef DEBUG
        std::cout << "Item " << i << " : " << getNodesRegExResults[D_ITEM_NODE] << std::endl;
#endif

        std::string dump = getNodesRegExResults[D_ITEM_NODE];
        boost::trim(dump);

        std::unique_ptr<ibf_node> node(new ibf_node(dump));
        if (!node->load()) {
            std::cerr << "Got error on loading dump string:" << std::endl << dump << std::endl;
            file.close();
            return false;
        }

        switch (action) {
        case D_ACTION_NODES_ALL:
            node_guids_ret = node_guids.insert(node->getNodeGUID());
            if (node_guids_ret.second==true) node->print();
            break;
        case D_ACTION_NODE:
            if (node->getNodeGUID() == param) {
                node->print();
                file.close();
                return true;
            }
            break;
        case D_ACTION_NODES_REG:
            try {
            boost::regex searchString(param);
            boost::smatch searchStringResults;
            if (boost::regex_match(node->getNodeGUID(), searchStringResults, searchString)){
                node_guids_ret = node_guids.insert(node->getNodeGUID());
                if (node_guids_ret.second==true) node->print();
            }
        } catch (boost::regex_error& e) {
                std::cerr << "Exception parsing regexp: " << e.what() << "\n";
                return false;
            }
            break;
        case D_ACTION_NODE_NEIGHBORS:
            if (node->getNodeGUID() == param) {
                max_ports = node->getPorts();

                uint32_t local_port = node->getLocalPort();

                // fill missing ports
                if (curr_port < local_port - 1) {
                    this->fill_missing_ports(curr_port + 1, local_port - 1);
                }

                curr_port = local_port;
#ifdef DEBUG
                std::cout << "Item " << i << " : " << getNodesRegExResults[D_ITEM_PEER] << std::endl;
#endif

                dump = getNodesRegExResults[D_ITEM_PEER];
                boost::trim(dump);

                std::unique_ptr<ibf_node> peer_node(new ibf_node(dump));
                if (!peer_node->load()) {
                    std::cerr << "Got error on loading dump string:" << std::endl << dump << std::endl;
                    file.close();
                    return false;
                }

#ifdef DEBUG
                std::cout << "Item " << i << " : " << getNodesRegExResults[D_ITEM_LINK_STATUS] << std::endl;
#endif

                dump = getNodesRegExResults[D_ITEM_LINK_STATUS];

                boost::regex_match(dump, linkStateExResults, linkStateEx);
                std::cout << node->getLocalPort() << "\t" << node->getLocalPortGUID() << "\t" << peer_node->getLocalPort() << "\t" << peer_node->getLocalPortGUID() << "\t" << linkStateExResults[D_LINK_STATUS] << std::endl;
            }
            break;
        }
    }

    switch (action) {
    case D_ACTION_NODE_NEIGHBORS:
        // fill missing ports
        curr_port++;
        if (curr_port <= max_ports)
            this->fill_missing_ports(curr_port, max_ports);
        break;
    }


    file.close();
    return true;
}

void ibf_parserer::fill_missing_ports(const uint32_t curr_port, const uint32_t max_port) const {
#ifdef DEBUG
    std::cout << curr_port << " " << max_port << std::endl;
#endif
    if (curr_port <= max_port) {
        for (uint32_t i=curr_port; i<=max_port; i++) {
            std::cout << i << "\tNONE\t\t\tNONE\tNONE\t\t\tDWN" << std::endl;
        }
    }
}
