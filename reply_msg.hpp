#ifndef REPLY_MSG
#define REPLY_MSG



#define ERR_NONICKNAMEGIVEN(nick, hostname) ":" + hostname + " 431 " + nick + " :No nickname given\r\n"

#define RPL_WELCOME(nick, hostname)  ":" + hostname + " 001 " + nick + " :Welcome " + nick + " to the ft_irc network !\r\n"
#define RPL_YOURHOST(nick, hostname) ":" + hostname + " 002 " + nick + " :Your host is " + hostname + " running version 1.0 !\r\n"
#define RPL_CREATED(nick, hostname, time_)  ":" + hostname + " 003 " + nick + " :This server was created " + time_ + " !\r\n"
#define RPL_MYINFO(nick, hostname)   ":" + hostname + " 004 " + nick + " :Host: " + hostname + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i, l !\r\n"


#define ERR_NEEDMOREPARAMS(nick, hostname) ":" + hostname + " 461 " + nick + " :Not enough parameters !\r\n" 
#define ERR_ALREADYREGISTERED(nick, hostname) ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"

#define ERR_NOTREGISTERED(nick, hostname) ":" + hostname + " 451 " + nick + " :You have not registered !\r\n"



// pourqoui \r\n
#endif