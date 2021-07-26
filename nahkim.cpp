#include <iostream>
#include "./include/Frame.hpp"
// 일치하는 정보 목록 반환
// <name> 매개 변수와 일치하는 정보 목록을 반환하는 쿼리를 생성하는 데 사용

void		Frame::cmdWho(Session *ss, std::vector<std::string> const& sets)
{
    // 402, (352, 318)
    // name에 매개 변수가 없으면 표시되는 모든 사용자(보이지않는 사용자 모드 + i)와
    // 요청 클라이언트와 공통 채널이 없는 사용자)가 나열됨
    // 동일한 결과는 가능한 모든 항목과 일치하는 name 또는 와일드카드를 사용하여 얻음
    // channal name을 찾을 수 없는 경우 사용자의 호스트,서버,실명 및 닉네임과 일치
    // "o" 매개 변수가 전달되면 제공된 이름 마스크에 따라 연산자만 변환

    // 채널 유저호스트
    UserMap::iterator itu;
    ChannelMap::iterator itc;
    std::vector<std::string> v;
    std::string res;
    std::string eol;

    itu = mUsers.begin();
    itc = mChannels.begin();
    eol = sets[1] + " :End of /WHO list";
    // channel user host server nick realname
    if (sets.size() == 1 || (sets.size() == 2 && sets[1] == "*"))   // 모든 유저 리스트 리턴
    {
        for (; itu != mUsers.end(); itu++,itc++)
        {
            res = itc->second->sName;
            res += itu->first + " " + itu->second->sHost + " " + "server " + itu->second->sNickname + "0 " + itu->second->sRealname + "\n";
            //RPL_WHOREPLY
            //"<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"
            ss->reply(res);
            res.clear();
        }
    }
    // else if (sets.size() == 2 && sets[1] == "o") // 옵션이 있을 경우
    // {
    //     // 관리자 리스트만 리턴
    //     for (;itu != mUsers.end(); itu++, itc++)
    //     {
    //         if (itu->second->manager == 1)
    //         {
    //             res = mChannels.sName;
    //             res += itu->first + " " + mUsers.sHost + " " + "server " + "0 " + mUsers.Realname + "\n";
    //             //RPL_WHOREPLY
    //             //"<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"
    //             ss->reply(res);
    //             res.clear();
    //         }
    //     }
    // }
    else
    {
        v = getMask(sets[1]);
        if (sets[2] == "o") // 옵션이 있을 경우
        {
            // 관리자 리스트만 리턴
            for (;itu != mUsers.end(); itu++, itc++)
            {
                if (itu->second->manager == 1)
                {
                    res = mChannels.sName;
                    res += itu->first + " " + mUsers.sHost + " " + "server " + "0 " + mUsers.Realname + "\n";
                    //RPL_WHOREPLY
                    //"<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"
                    ss->reply(res);
                    res.clear();
                }
            }
            // wild card 있을 경우
        }
        else    // 옵션이 없는 경우
        {
            if (v.empty())  // wild card 없을 경우
            {
                if (doesChannelExists(sets[1]))     // channel
                {
                }
                else if (mUsers->second->sHost.find(sets[1]) == mUsers->second->sHost.end())   // host
                {
                }
                else if (server.find(sets[1]) == server.end())       // server
                    return ss->reply("402");   // ERR_NOSUCHSERVER
                else if (mUsers.find(sets[1]) == mUsers.end())       // real name
                {
                }
                else if (doesNicknameExists(sets[1]))            // nick name
                {
                    for (; itu != itu.end(); itu++,itc++)
                {
                    res = itc->second->sName;
                    res += itu->first + " " + itu->second->sHost + " " + "server " + "0 " + itu->second->sRealname + "\n";
                    //RPL_WHOREPLY
                    //"<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"
                    ss->reply(res);
                    res.clear();
                }
                }
            }
        }
        //RPL_WHOREPLY
        //"<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"

        return ss->reply(eol);
        //RPL_ENDOFWHO
        //"<name> :End of /WHO list"
        }
    
}

void		Frame::cmdWhois(Session *ss, std::vector<std::string> const& sets)
{
    // error 402 -431 (311 312 313 317 318 319 301) -401
    if (sets.size() == 1)
        ss->reply("431");   // ERR_NONICKNAMEGIVEN
    else if (!doesNicknameExists(sets[1]))
		ss->reply("401"); //ERR_NOSUCHNICK
    // else if (server.find(set[1]) == server.end())
    //     ss->reply("402");   // ERR_NOSUCHSERVER

    // 311     RPL_WHOISUSER
    //                     "<nick> <user> <host> * :<real name>"
    //     312     RPL_WHOISSERVER
    //                     "<nick> <server> :<server info>"
    //     313     RPL_WHOISOPERATOR
    //                     "<nick> :is an IRC operator"
    //     317     RPL_WHOISIDLE
    //                     "<nick> <integer> :seconds idle"
    //     318     RPL_ENDOFWHOIS
    //                     "<nick> :End of /WHOIS list"
    //     319     RPL_WHOISCHANNELS
    //                     "<nick> :{[@|+]<channel><space>}"
    
}

void		Frame::cmdWhowas(Session *ss, std::vector<std::string> const& sets)
{
    // (369) -431
    std::string res;
    std::list<std::string>::iterator it;

    if (sets.size() == 1)
    {
        ss->reply("431");   // ERR_NONICKNAMEGIVEN
    }
    //mUser.find(sets[1]) != mUser.end()
    it = mUsers.second->_pastnick.rbegin();
    for (; it != mUsers.second->_pastnick.rend(); it++)
    {
        if (sets[1] == it)
    }

    if (mUsers.second->_pastNick.find(sets[1]) == mUsers.second->_pastNick.end())
        ss->reply(sets[1] + " :End of WHOWAS");   // RPL_ENDOFWHOWAS
    //"<nick> :End of WHOWAS"

}

void		Frame::cmdPrivmsg(Session *ss, std::vector<std::string> const& sets)
{
    // (301) -401 -407 -411 -412 413 414 -404

    int i = 2;
    std::string res;
    std::vector<std::string> receivers;
    std::vector<std::string> msg;
    std::vector<std::string>::iterator msgit = msg.begin();

    if (sets[1][0] == ':')
    {
        ss->reply("411");   // ERR_NORECIPIENT
    }
    else if (sets[2][0] != ':')
    {
        ss->reply("412");   // ERR_NOTEXTTOSEND
    }
    // receiver에 콤마로 split해서 저장하기
    receivers = split_comma(sets[1]);
    std::vector<std::string>::iterator receiverit = receivers.begin();

    // 메세지 전까지 확인하기
    while (receiverit != receivers.end())
    {
        if (sets[1][0] == '#')  // channel
        {
            if (!doesChannelExists(*receiverit)) // mChannels.second->sName
                ss->reply("404");   // ERR_CANNOTSENDTOCHAN
        }
        else
        {
            if (!doesNicknameExists(*receiverit))
                ss->reply("401");       // ERR_NOSUCHNICK
            if (receiverit + 1 != receivers.end())
            {
                if (*receiverit == *(receiverit + 1))
                    ss->reply("407");       // ERR_TOOMANYTARGETS
            }
        }
        receiverit++;
    }

    // 문자열 붙이기 :부터 맨 마지막까지
    res = (std::string)sets.at(i);
    std::string receiver;
    while (++i < sets.size())
    {
        res += " " + sets.at(i);   // vector을 문자열로 변환
    }
    receiverit = receivers.begin();
    while (receiverit != receivers.end())
    {
        ss->reply(*receiverit + " " + res);
        receiverit++;
    }
    //ss->reply("<nick> :<away message>";

}

