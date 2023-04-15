/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canalManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:15:15 by dasereno          #+#    #+#             */
/*   Updated: 2023/03/15 17:16:06 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.h"

class CanalManager {
    public:
        CanalManager( void );
        ~CanalManager( void );
        Canal *CreateChannel(std::string, Client *);
        Canal *GetChannel(std::string);
        void DeleteChannel(std::string);
        void DeleteChannel(Canal *);
        void DeleteAllChannels();
        std::map<std::string, Canal *> GetChannels();
    
    private:
        std::map<std::string, Canal *> _channels;
};