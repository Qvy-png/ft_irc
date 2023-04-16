/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canalManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:17:05 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/16 19:33:56 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

CanalManager::CanalManager() {}
CanalManager::~CanalManager() {
    DeleteAllChannels();
}

Canal *CanalManager::CreateChannel(std::string name, Client *client) {
    Canal *channel = new Canal(name, *client, this);
    _channels[name] = channel;
    channel->pushClient(client);
    return channel;
}

Canal *CanalManager::GetChannel(std::string name) {
    if (!_channels[name]) {
        _channels.erase(name);
        return NULL;
    }
    return _channels[name];
}

void CanalManager::DeleteChannel(std::string name) {
    delete _channels[name];
    _channels.erase(name);
}

void CanalManager::DeleteChannel(Canal *channel) {
    for (std::map<std::string, Canal *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
        if (it->second == channel) {
            _channels.erase(it);
            break;
        }
    }
    delete channel;
}

void CanalManager::DeleteAllChannels() {
    for (std::map<std::string, Canal *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
        if (!it->second) continue;
        delete it->second;
    }
    _channels.clear();
}

std::map<std::string, Canal *> CanalManager::GetChannels() {
    return _channels;
}
