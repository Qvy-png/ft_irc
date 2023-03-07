/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:14:42 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/07 16:33:47 by rdel-agu         ###   ########.fr       */
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
        void        setPass( std::string );
        void        setNick( std::string );
        void        setUser( std::string );
        void        setFullName( std::string );
        void        setBuffer( char* );
        int         getFd( void );
        std::string getPass( void );
        std::string getNick( void );
        std::string getUser( void );
        std::string getFullName( void );
        std::string getBuffer( void );
};

#endif