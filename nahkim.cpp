void	Frame::printcommand(Session *ss)
{
	ss->replyAsServer("<Command>");
	//ss->replyAsServer("WHO <nick> [o]");
	//ss->replyAsServer("WHOIS <nickmask>[,<nickmask>]");
	//ss->replyAsServer("PRIVMSG <receiver>[,<receiver>] <text to be send>");
	ss->replyAsServer("JOIN <channel>[,<channel>] [<key>[,<key>]");
	ss->replyAsServer("PART <channel>[,<channel>]");
	ss->replyAsServer("TOPIC <channel> [<topic>]");
	//ss->replyAsServer("NAMES");
	ss->replyAsServer("LIST [<channel>[,<channel>]]");
	ss->replyAsServer("INVITE <nick> <channel>");
	ss->replyAsServer("KICK <channel> <user> [<comment>]");
	//ss->replyAsServer("NICK");
	//ss->replyAsServer("USER");
	//ss->replyAsServer("PASS");
	//ss->replyAsServer("OPER");
	//ss->replyAsServer("QUIT");
}
