/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:14:42 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/14 10:52:13 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client {
    
    private:

        int         _fd;
        bool        _hs;
        std::string _pass;
        std::string _nick;
        std::string _host;
        std::string _fullName;
        std::string _buffer;
        std::time_t _time;

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
        void        setTime( std::time_t );

        int         getFd( void );
        bool        getHs( void );
        std::string getPass( void );
        std::string getNick( void );
        std::string getHost( void );
        std::string getFullName( void );
        std::string getBuffer( void );
        std::time_t getTime( void );
};

#endif
