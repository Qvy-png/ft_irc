/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:14:42 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/04/16 19:00:28 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "canal.hpp"
#include "ft_irc.h"

class Canal;
class Message;

class Client {
    
    private:
        int                         _fd;
        bool                        _hs;
        std::string                 _pass;
        std::string                 _nick;
        std::string                 _host;
        std::string                 _fullName;
        std::string                 _buffer;
        std::vector<Canal *>        _canals;
        std::vector<Message *>      _waitingMessages;
        bool                        _op;
        bool                        _hasTime;
        std::time_t                 _time;
        int                         _modeI;
        

    public:
        Client( void );
        Client( const Client& );
        Client( int read );
        ~Client( );
        
        Client&     operator=( const Client& );

        int	send_msg(std::string msg) {
			int res;
			
			res = send(_fd, msg.c_str(), msg.length(), MSG_CONFIRM);
			return (res);
		}
        
        void        setFd( int );
        void        setHs( bool );
        void        setPass( std::string );
        void        setNick( std::string );
        void        setHost( std::string  );
        void        setFullName( std::string  );
        void        setBuffer( char* );
        void        addBuffer( char* );
        void        pushMessage(Message *msg) { _waitingMessages.push_back(msg); };
        int         getFd( void );
        bool        getHs( void );
        bool        getOp( void) { return _op; };
        void        setOp( bool op ) { _op = op; };
        void        setTime( std::time_t );
        void        setHasTime( bool );
        void        setModeI( bool val ) { _modeI = val; };
        
        std::string getPrefix( void ) { return _nick + (_fullName.empty() ? "" : "!" + _fullName) + (_host.empty() ? "" : "@" + _host);};
        std::string getPass( void );
        std::string getNick( void );
        std::string getHost( void );
        std::string getFullName( void );
        std::string getBuffer( void );
        std::time_t getTime( void );
        bool        getHasTime( void );
        bool        getModeI( void ) { return _modeI; };
};

#endif
