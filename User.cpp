#include "include/User.hpp"
#include "include/Session.hpp"
#include "include/Frame.hpp"

User::User(Session*	ms)
	: mysession(ms), didUser(false), didNick(false), is_properly_quit(false)
{}

User::~User()
{
	if (!is_properly_quit)
	{
		/*
		ChannelMap::iterator it;

		it = mChannels.begin()
		for (; it != mChaneels.end() ; ++it)
		{
			(*it).removeUser(this);
			mChaneels.erase(*it);
		}
		Mainframe::instance()->removeUser(sNickName);
		Mainframe::instance()->updateChannels();
		 */
	}
}

User*			User::create(Session *ms)
{
	return new User(ms);
}

void			User::setName(std::string const& s)
{
	sRealname = s;
}
void			User::setNick(std::string const& s)
{
	sNickname = s;
}
void			User::setHost(std::string const& s)
{
	sHost = s;
}
std::string		User::name() const
{
	return (sRealname);
}

std::string		User::nick() const
{
	return (sNickname);
}

std::string		User::Host() const
{
	return (sHost);
}

std::string		User::msgHeader() const
{
	return (std::string(":" + sNickname + "!" + sNickname + "@" + sHost+ " "));
}

bool	User::CheckNick() const
{
	return didNick;
}

bool	User::CheckUser() const
{
	return didUser;
}

bool	User::addNick(std::vector<std::string> const& sets)
{
	if (didNick)
		return false;
	sNickname = sets[1];
	didNick = true;
	return true;
}

void	User::cmdNick(std::vector<std::string> const& sets)
{
	_pastNick.insert(_pastNick.end(), sNickname);
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
		it->second->cmdNick(sNickname, sets[2]);
	sNickname = sets[2];
	/*
	bool res;
	std::string s = sets[1];

	if (sets[0] != "NICK")
	{
		if (&sets[0][1] != sNickname)
			return ;
		s = sets[2];
	}
	res = Frame::instance()->doesNicknameExists(s);
	if (res)
		return ; // fail
	if (didNick)
	{
		if (sets[0] == "NICK")
			return ;
		_pastNick.insert(_pastNick.end(), sNickname);
		//이전 닉네임 저장
	}
	sNickname = s;
	didNick = true;
	//channel 에 있는 usermap의 키 닉네임도 바꿔야함. 
	//frame에 있는 usermap의 키 닉네임도 바꿔야함. 
	return ; //success
	*/
}

bool		User::cmdUser()
{
	if (didNick == false)
		return false;
	didUser = true;
	return true;
}

void			User::cmdJoin(std::pair<std::string, Channel *ch> it)
{
	/*
	//write
	bool	res;
	std::string s = sets[1];
	ChannelMap::iterator	it;
	
	res = Frame::instance()->doesChannelExists(s);
	if (res)
	{
		// channel 이미 존재 찾아서 user 추가
		//Frame::instance()->findChannel(s)->second->addUser(this);
		it = Frame::instance()->findChannel(s);
		it->second->addUser(this);
		mChannels.insert(*it);
	}
	else
	{
		Channel *new_chan = new Channel(this, s);
		Frame::instance()->addChannel(new_chan);
	}
	 */
	mChannels.insert(it);
}

void			User::cmdKick(std::vector<std::string> const& sets)
{
	//write
}

void			User::cmdPart(std::vector<std::string> const& sets)
{
	ChannelMap::iterator	it;

	it = mChannels.find(sets[1]);
	it->second->removeUser(this);
	mChannels.erase(it);
}

void			User::cmdQuit(std::vector<std::string> const& sets)
{
	_pastNick.clear();
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
		it->second->removeUser(this);
	mChannels.clear();
}

