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

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include "memory"

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sysexits.h>

#include <boost/regex.hpp>

#include "config.hpp"
#include "core/utils.hpp"
#include "core/ibf_parser.hpp"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

/**
 * @brief help
 * Display help information.
 */
void help(void) {
    std::cout << "Usage: ibf_tool [ARGUMENTS]..." << std::endl;
    std::cout << "InfiniBand fabric dump explore tool." << std::endl << std::endl;
    std::cout << "Mandatory arguments:" << std::endl;
    std::cout << "\t-f\t\t: Text file in LST format describing InfiniBand fabric" << std::endl << std::endl;
    std::cout << "Exit status:" << std::endl;
    std::cout << " 0  if OK," << std::endl;
    std::cout << " 1  if minor problems (e.g., cannot access subdirectory)," << std::endl;
    std::cout << " 2  if serious trouble (e.g., cannot access command-line argument)." << std::endl;

}

void valid_commands(void) {
    std::cerr << "Valid commands are:" << std::endl;
    std::cerr << "\tshow nodes all" << std::endl;
    std::cerr << "\tshow node <node_GUID>" << std::endl;
    std::cerr << "\tshow nodes <reg_exp>" << std::endl;
    std::cerr << "\texit" << std::endl;
}

int main(int argc, char *argv[])
{
    std::string data_file = "" ;
    int c = 0;

    while( ( c = getopt (argc, argv, "f:") ) != -1 )
    {
        switch(c)
        {
            case 'f':
                if(optarg) data_file = optarg;
                break;
            default:
                help();
                return EX_USAGE;
                break;
        }
    }

    if (data_file.empty()){
        help();
        return EX_USAGE;
    }

    std::cout << "using ibf dump file: " << data_file << "!\n" ;

    std::unique_ptr<ibf_parserer> parser_(new ibf_parserer(data_file));
    if (!parser_->load())
        return EX_DATAERR;

    boost::regex getCommandEx("^(\\w+)|((\\w+) (\\w+) (.*))$");
    boost::smatch getCommandExResults;

    std::string command;
    while(true){
        std::cout << "ibtool# ";
        std::getline(std::cin, command);

        if (boost::regex_match(command, getCommandExResults, getCommandEx)){
            std::string cmd = getCommandExResults[1];
            std::string action = getCommandExResults[2];
            std::string param = getCommandExResults[3];
            if (cmd == "show") {
                if (action == "node") {
                    parser_->parse(D_ACTION_NODE, param);
                } else if (action == "nodes") {
                    if (param == "all") {
                        parser_->parse(D_ACTION_NODES_ALL);
                    } else {
                        parser_->parse(D_ACTION_NODES_REG, param);
                    }
                } else {
                    valid_commands();
                }
            } else if (cmd == "exit") {
                  break;
            } else {
                    valid_commands();
            }
        } else {
            valid_commands();
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    return EX_OK;
}
