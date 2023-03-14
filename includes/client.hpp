/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:14:42 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/14 21:00:53 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "canal.hpp"
#include "ft_irc.h"

class Canal;

class Client {
    
    private:

        int         _fd;
        bool        _hs;
        std::string _pass;
        std::string _nick;
        std::string _host;
        std::string _fullName;
        std::string _buffer;
        std::vector<Canal *> _canals;

    public:
        Client( void );
        Client( const Client& );
        Client( int read );
        ~Client( );
        
        Client& operator=( const Client& );
        
        void        setFd( int );
        void        setHs( bool );
        void        setPass( std::string );
        void        setNick( std::string );
        void        setHost( std::string  );
        void        setFullName( std::string  );
        void        setBuffer( char* );
        void        addBuffer( char* );
        void        addCanal( Canal *canal) { _canals.push_back(canal); };
        std::vector<Canal *> getCanalList(void) { return (_canals); };
        void        printCanals(void);
        bool        isInCanal(std::string canalName);
        Canal       *getCanal(std::string canalName);
        void        exitCanal(std::string canalName);
        int         getFd( void );
        bool        getHs( void );
        std::string getPass( void );
        std::string getNick( void );
        std::string getHost( void );
        std::string getFullName( void );
        std::string getBuffer( void );
};

#endif
