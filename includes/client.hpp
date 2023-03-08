/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:14:42 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/08 10:08:40 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client {
    
    private:

        int         _fd;
        std::string _pass;
        std::string _nick;
        std::string _user;
        std::string _fullName;
        std::string _buffer;

    public:

        Client( void );
        Client( const Client& );
        Client( int read ); 
        ~Client( );
        
        Client& operator=( const Client& );
        
        void        setFd( int );
        void        setPass( char* );
        void        setNick( char*);
        void        setUser( char* );
        void        setFullName( char* );
        void        setBuffer( char* );
        int         getFd( void );
        std::string getPass( void );
        std::string getNick( void );
        std::string getUser( void );
        std::string getFullName( void );
        std::string getBuffer( void );
};

#endif
