#ifndef REPLY_MSG
#define REPLY_MSG


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define RPL_JOIN(nick, username, channelname, ipaddress)                    ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
#define RPL_NAMREPLY(hostname, clients, channelname, nick)                  ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
#define RPL_ENDOFNAMES(hostname, nick, channelname)                         ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"


#define ERR_BADCHANNELKEY(buffer, channel)  "475 " + buffer + " " + channel + " :Cannot join channel (+k)\r\n"

#define ERR_NONICKNAMEGIVEN(nick, hostname) ":" + hostname + " 431 " + nick + " :No nickname given\r\n"

#define RPL_WELCOME(nick, hostname)  ":" + hostname + " 001 " + nick + " :Welcome " + nick + " to the ft_irc network !\r\n"
#define RPL_YOURHOST(nick, hostname) ":" + hostname + " 002 " + nick + " :Your host is " + hostname + " running version 1.0 !\r\n"
#define RPL_CREATED(nick, hostname, time_)  ":" + hostname + " 003 " + nick + " :This server was created " + time_ + " !\r\n"
#define RPL_MYINFO(nick, hostname)   ":" + hostname + " 004 " + nick + " :Host: " + hostname + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i, l !\r\n"
#define RPL_TOPIC(hostname, nick, channel, topic)                    ":" + hostname + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_TOPICWHOTIME(client, channel, nick, seter,username, setat)                      ":" + client + " 333 " + nick + " " + channel + " " + seter + "!~@oldschool_irc :" + setat + "\r\n"


#define RPL_ENDOFNAMES(hostname, nick, channelname)                         ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"
#define RPL_JOIN(nick, username, channelname, ipaddress)                    ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
#define RPL_NAMREPLY(hostname, clients, channelname, nick)                  ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"

#define ERR_NEEDMOREPARAMS(nick, hostname) ":" + hostname + " 461 " + nick + " :Not enough parameters !\r\n" 
#define ERR_ALREADYREGISTERED(nick, hostname) ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"

#define ERR_NOTREGISTERED(nick, hostname) ":" + hostname + " 451 " + nick + " :You have not registered !\r\n"

#define ERR_NOSUCHCHANNEL(hostname, channel, nick) ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"


#define ERR_NOTOP(hostname, channel) ":" + hostname + " 482 " + channel + " " + ":You're not a channel operator\r\n"
#define ERR_NOSUCHNICK(hostname, channel, argument) ":" + hostname + " 401 " + channel + " " + argument + " :No such nick/channel\r\n"


#define RPL_KICK(nick, username, host, kicked, channel, commntaire) ":" + nick + "!" + "~" + username + "@" + host + " KICK " + channel + " " + kicked + " " + commntaire + "\r\n"

#define RPL_NOTOPIC(hostname, nick, chann)   ":" + hostname + " 331 " + nick + " " + chann + " :No topic is set.\r\n"
#define RPL_TOPICDISPLAY(hostname, nick, channel, topic)                    ":" + hostname + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_TOPICWHOTIME(client, channel, nick, seter,username, setat)                      ":" + client + " 333 " + nick + " " + channel + " " + seter + "!~@oldschool_irc :" + setat + "\r\n"
#define ERR_NOTONCHANNEL(client, channel)            "442 " + client + " " + channel + " :You're not on that channel\r\n"
#define ERR_CHANOPRIVSNEEDED(buffer, channel)                               ":" + buffer + " 482 " + channel + " :You're not channel operator\r\n"
#define RPL_NEWTOPICSETTED(nick, username, hostname, channel, topic)        ":" + nick + "!~" + username + "@" + hostname + " TOPIC " + channel + " " + topic + "\r\n"

// pourqoui \r\n
#endif