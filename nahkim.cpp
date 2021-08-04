void	Frame::printcommand(Session *ss)
{
	std::string res;

	res += "PRIVMSG ";
	res += ss->user().nick();
	res += " :";
	ss->replyAsServer(res +"<Command>");
	//ss->replyAsServer("WHO <nick> [o]");
	//ss->replyAsServer("WHOIS <nickmask>[,<nickmask>]");
	//ss->replyAsServer("PRIVMSG <receiver>[,<receiver>] <text to be send>");
	ss->replyAsServer(res + "JOIN <channel>[,<channel>] [<key>[,<key>]");
	ss->replyAsServer(res + "PART <channel>[,<channel>]");
	ss->replyAsServer(res + "TOPIC <channel> [<topic>]");
	//ss->replyAsServer("NAMES");
	ss->replyAsServer(res + "LIST [<channel>[,<channel>]]");
	ss->replyAsServer(res + "INVITE <nick> <channel>");
	ss->replyAsServer(res + "KICK <channel> <user> [<comment>]");
	//ss->replyAsServer("NICK");
	//ss->replyAsServer("USER");
	//ss->replyAsServer("PASS");
	//ss->replyAsServer("OPER");
	//ss->replyAsServer("QUIT");
}
