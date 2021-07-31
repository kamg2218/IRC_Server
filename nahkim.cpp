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

    UserMap::iterator itu;
    ChannelMap::iterator itc;
    std::vector<std::string> v;
    std::string res;
    std::string eol;
    std::string member;
    int i = 0;

    itu = mUsers.begin();
    itc = mChannels.begin();
    eol = sets[1] + " :End of /WHO list";
    if (sets.size() == 1 || (sets.size() == 2 && sets[1] == "*"))
    {
        for (; itu != mUsers.end(); itu++,itc++)
        {
            res = itc->second->name();
            res += itu->first + " " + itu->second->user().host() + " " + "server " + itu->second->user().nick() + "0 " + itu->second->user().name();
            ss->replyAsServer(res);         // RPL_WHOREPLY
            res.clear();
        }
    }
    else
    {
        v = getMask(sets[1]);
        for (; i < v.size(); i++)
        {
            member = v[i];
            if (member == "#" && doesChannelExists(v[i]))     // channel
            {
                // v[i]이 채널인 유저 정보 가져오기
                // # 제거 함수 필요
                for (itc = mChannels.begin(); itc != mChannels.end(); itc++)
                {
                    if (itc->first == v[i].substr(1))
                    {
                        for (itu = itc->second.mUsers.begin(); itu != itc->second.mUsers.end(); itu++)
                        {
                            res = itc->first + " " + itc->second.mUsers->second.user() + " " + itc->second.mUsers->second.user().host() + " " + "server " + "0 " + itc->second.mUsers->second.user().nick();
                            if (sets[1] == "o") // 옵션이 있는 경우
                            {
                                if (itc->second.mUsers->second.user().manager == 1)
                                    ss->replyAsServer(res); // RPL_WHOREPLY
                            }
                            else
                            {
                                ss->replyAsServer(res); // RPL_WHOREPLY
                            }
                            res.clear();
                        }
                    }
                }
            }
            else if (mUsers->second.user().host().find(sets[1]) == mUsers->second.user().host().end())   // host(client의 hostname)
            {
                for (itu = mUsers.begin(); itu != mUsers.end(); itu++)
                {
                    if (itu->second.user().host() == v[i])
                    {
                        if (sets[1] == "o") // 옵션이 있는 경우
                        {
                            if (itu->second.user().manager == 1)
                                ss->replyAsServer(res); // RPL_WHOREPLY
                        }
                        else
                        {
                            res = itu->second.user().mChannels->first + " " + itu->first + " " + itu->second.user().host() + " " + "server " + "0 " + itu->second.user().name();
                            ss->replyAsServer(res); // RPL_WHOREPLY
                        }
                        res.clear();
                    }
                    
                }
            }
            // else if (server.find(sets[1]) == server.end())       // server
            //     return ss->reply("402");   // ERR_NOSUCHSERVER
            else if (mUsers.find(v[i]) != mUsers.end())       // real name
            {
                for (itu = mUsers.begin(); itu != mUsers.end(); itu++)
                {
                    if (itu->second.user().name() == v[i])
                    {
                        if (sets[1] == "o")
                        {
                            if (itu->second.user().manager == 1)
                                ss->replyAsServer(res); // RPL_WHOREPLY
                        }
                        else
                        {
                            res = itu->second.user().mChannels->first + " " + itu->first + " " + itu->second.user().host() + " " + "server " + "0 " + itu->second.user().name();
                            ss->replyAsServer(res); // RPL_WHOREPLY
                        }
                        res.clear();
                    }
                    
                }
            }
            else if (doesNicknameExists(sets[1]))            // nick name
            {
                for (itu = mUsers.begin(); itu != mUsers.end(); itu++)
                {
                    if (itu->second.user().nick() == v[i])
                    {
                        if (sets[1] == "o")
                        {
                            if (itu->second.user().manager == 1)
                                ss->replyAsServer(res); // RPL_WHOREPLY
                        }
                        else
                        {
                            res = itu->second.user().mChannels->first + " " + itu->first + " " + itu->second.user().host() + " " + "server " + "0 " + itu->second.user().name();
                            ss->replyAsServer(res); // RPL_WHOREPLY
                        }
                        res.clear();
                    }
                    
                }
            }
            member.clear();
        }
    }
    ss->replyAsServer(eol); // RPL_ENDOFWHO
}

void		Frame::cmdWhois(Session *ss, std::vector<std::string> const& sets)
{
    std::string eol;
    std::string res;
    std::vector<std::string> v;

    if (sets.size() == 1)
        ss->Err_431();   // ERR_NONICKNAMEGIVEN
    v = split_comma(sets[1]);
    for (int i = 0; i < v.size(); i++)
    {
        if (!doesNicknameExists(v[i]))
		    ss->Err_401(v[i]); //ERR_NOSUCHNICK
    }
    eol = sets[1] + " :End of /WHOIS list";
    for (int i = 0; i < v.size(); i++)  // -> iterator 사용할까..
    {
        res += v[i] + " " + v[i].user + " " + v[i].host + " :" + v[i].realname; 
        ss->replyAsServer(res);        // RPL_WHOISUSER
        res.clear();
        if ()// irc 관리자일 경우 -> 어케암?
        {
            res = v[i] + " :is an IRC operator";
            ss->replyAsServer(res);    // RPL_WHOISOPERATOR
            res.clear();
        }
        res = "<nick> <server> :<server info>";
        ss->replyAsServer(res);    // RPL_WHOISSERVER -> 서버 이름이 무엇?
        res.clear();
        res = "<nick> <integer> :seconds idle";
        ss->replyAsServer(res);    // RPL_WHOISIDLE -> ???
        res.clear();
        if (v[i]->manager == 1)       // 채널 관리자인 경우
        {
            res = v[i] + " :@" + v[i].channel + " ";
        }
        else
        {
            res = v[i] + " :" + v[i].channel + " ";
        }
        ss->replyAsServer(res);    // RPL_WHOISCHANNELS
    }
    ss->replyAsServer(eol);  // RPL_ENDOFWHOIS
    // RPL_AWAY? 어디다씀..
    
}

void		Frame::cmdWhowas(Session *ss, std::vector<std::string> const& sets) // 에러나 응답하면 무조건 endofwhowas 넣기
{
    std::string res;
    std::vector<std::string>::reverse_iterator itv;
    std::list<std::string>::reverse_iterator itl;
    int count = -1;
    bool check = 0;

    if (sets.size() == 1)
    {
        ss->Err_431();   // ERR_NONICKNAMEGIVEN
        ss->replyAsServer(sets[1] + " :End of WHOWAS"); // RPL_ENDOFWHOWAS
        return ;
    }
    else if (sets.size() > 2)
    {
        count = stoi(sets[2]);  // 만들어야하나
    }
    // 모든 유저에 대한 _pastNickname을 확인(list)
    for (itv = mUsers.rbegin(); itv != mUsers.rend(); itv++)
    {
        if (count == 0)
            break;
        for (itl = itv._pastNick.rbegin(); itl != itv->_pastnick.rend(); itl++)
        {
            if (*itl == sets[1])
            {
                res = itv->user().nick() + " " + itv->user().user() + " " + itv->user().host() + " * :" + itv->user().name();
                ss->replyAsServer(res);    // RPL_WHOWASUSER
                count--;
                check = 1;
            }
        }
    }
    if (!check)
    {
        ss->Err_406(sets[1]);   // ERR_WASNOSUCHNICK
    }                 
    ss->replyAsServer(sets[1] + " :End of WHOWAS"); // RPL_ENDOFWHOWAS
    //"<nick> :End of WHOWAS"

}

void		Frame::cmdPrivmsg(Session *ss, std::vector<std::string> const& sets)
{
    int i = 2;
    std::string res;
    std::vector<std::string> receivers;
    std::vector<std::string> msg;
    std::vector<std::string>::iterator msgit = msg.begin();
    UserMap::iterator itu;
    ChannelMap::iterator itc;

    if (sets[1][0] == ':')
    {
        ss->Err_411(sets[0]);   // ERR_NORECIPIENT
    }
    else if (sets[2][0] != ':')
    {
        ss->Err_412();   // ERR_NOTEXTTOSEND
    }
    // receiver에 콤마로 split해서 저장하기
    receivers = split_comma(sets[1]);
    std::vector<std::string>::iterator receiverit = receivers.begin();
    std::vector<std::string>::iterator tmp;
    UserMap::iterator itu;
    itu = mUsers.begin();

    // 메세지 전까지 확인하기
    while (receiverit != receivers.end())
    {
        if (sets[1][0] == '#')  // channel
        {
            if (!doesChannelExists((*receiverit).substr(1))) // mChannels.second->sName
                ss->Err_404(*receiverit);   // ERR_CANNOTSENDTOCHAN
        }
        else
        {
            if (!doesNicknameExists(*receiverit))
                ss->Err_401(*receiverit);       // ERR_NOSUCHNICK
            for (tmp = receiverit + 1; tmp != receivers.end(); tmp++)
            {
                if (*receiverit == *(receiverit + 1))
                    ss->Err_407(*receiverit);       // ERR_TOOMANYTARGETS
            }
        }
        receiverit++;
    }
    std::string receiver;
    for (receiverit = receivers.begin(); receiverit != receivers.end(); receiverit++)
    {
        receiver = *receiverit;
        if (receiver[0] == '#')
        {
            for (;itc != mChannels.end(); receiverit++)
            {
                itc = mChannels.find((*receiverit).substr(1));
                if (itc->second.name() == *receiverit)
                    itc->second->broadcast(ss ,sets[2]);
            }
        }
        else
        {
			for (itu = mUser.begin(); itu != mUsers.end(); itu++)
            {
                if (itu->second.user().name() == *receiverit)
                    ss->replyAsUser(itu->second, sets[2]);
            }
        }
    }
}


