#include <iostream>
#include "Frame.hpp"
// 일치하는 정보 목록 반환
// <name> 매개 변수와 일치하는 정보 목록을 반환하는 쿼리를 생성하는 데 사용
void		Frame::cmdWho(Session *ss, std::vector<std::string> const& sets)
{
    // error 402, 352, 318
    // name 에 매개 변수가 없으면 표시되는 모든 사용자(보이지않는 사용자 모드 + i)와
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
    if (sets.size() > 3)    // error
    {
        ss->reply("Error :too many arguments");    // reply로 해도됨?
    }
    eol = sets[1] + " :End of /WHO list";
    // channel user host server nick realname
    if (sets.size() == 1)   // 모든 유저 리스트 리턴
    {
        for (; itu != itu.end(); itu++,itc++)
        {
            res = itc->second->Sname;
            res += "user이름이 무엇임..? " + itu->second->sHost + " " + "server " + "0 " + itu->second->Realname + "\n";
            //RPL_WHOREPLY
            //"<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"
            ss->reply(res);
            res.clear();
        }
        return ss->reply(eol);
        return ;
    }
    
    v = getMask(sets[1]);
    if (sets[2] == "o") // 옵션이 있을 경우
    {
        // 관리자 리스트만 리턴
        for (;itu != itu.end(); itu++, itc++)
        {
            if (itu->second->manager == 1)
            {
                res = mChannels.Sname;
                res += "user이름이 무엇임..? " + mUsers.sHost + " " + "server " + "0 " + mUsers.Realname + "\n";
                //RPL_WHOREPLYß
                //"<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"
                ss->reply(res);
                res.clear();
            }
        }
        // wild card 있을 경우
    }
    else
    {
        if (v == NULL)  // wild card 없을 경우
        {
            if (doesChannelExists(sets[1]))     // channel
            {
            }
            if (userhost.find(sets[1]) == userhost.end())   // host
            {
            }
            if (server.find(sets[1]) == server.end())       // server
                return ss->reply("402");   // ERR_NOSUCHSERVER
            if (mUsers.find(sets[1]) == mUsers.end())       // real name
            {
            }
            if (doesNicknameExists(sets[1]))            // nick name
            {
            }
        }
        else
        {
        }
    }
    //RPL_WHOREPLY
    //"<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"

    return ss->reply(eol);
    //RPL_ENDOFWHO
    //"<name> :End of /WHO list"
}

void		Frame::cmdWhoi(Session *ss, std::vector<std::string> const& sets)
{
    // error 402 431 311 312 313 317 318 319 301 401
    if (server.find(set[1]) == server.end())
        ss->reply("402");   // ERR_NOSUCHSERVER
    if (mUsers.find(sets[0]) == mUsers.end())
        ss->reply("431");   // ERR_NONICKNAMEGIVEN
    
}

void		Frame::cmdWhowas(Session *ss, std::vector<std::string> const& sets)
{
    // ERROR 369 431
    std::string res;
    if (sets.size() == 1 || )
    {
        ss->reply("431");   // ERR_NONICKNAMEGIVEN
    }
    if (mUsers.second->Sname.find(sets[0]) == mUsers.second->Sname.end())
        ss->reply(sets[1] + " :End of WHOWAS");   // RPL_ENDOFWHOWAS
    //"<nick> :End of WHOWAS"

}

void		Frame::cmdPrivmsg(Session *ss, std::vector<std::string> const& sets)
{
    // ERROR 301 -401 407 -411 -412 413 414 -404

    int i = 2;
    std::string res;
    std::vector<std::string>::iterator it = sets.begin();
    std::vector<std::string> receiver;
    std::vector<std::string>::iterator receiverit = receiver.begin();
    std::vector<std::string> msg;
    std::vector<std::string>::iterator msgit = msg.begin();

    if (sets[1][0] == ':')   // receiver가 없을 경우
    {
        ss->reply("411");   // ERR_NORECIPIENT
    }
    else if (sets[2][0] != ':')
    {
        ss->reply("412");   // ERR_NOSUCHSERVER
    }
    // receiver에 콤마로 split해서 저장하기


    // 메세지 전까지 확인하기
    while (receiverit != receiver.end())
    {
        if (mUsers.second->sNickname.find(sets[i]) == mUsers.second->sNickname.end())   // mUsers.second->sNickname
            ss->reply("401");       // ERR_NOSUCHNICK
        if (mChannels.second->Sname.find(sets[i]) == mChannels.second->Sname.end()) // mChannels.second->Sname
            ss->reply("404");   // ERR_CANNOTSENDTOCHAN

        // ?중복 수신자인지 확인
    }

    // channel @ # $
    // 문자열 붙이기 :부터 맨 마지막까지
    std::string res = (std::string)sets.at(i);  // vector을 문자열로 변환
    it = sets.begin() + i;
    while (it != sets.end())
    {
        res += " " + sets.at(i);   // vector을 문자열로 변환
        i++;
        it++;
    }
    receiverit = receiver.begin();
    while (receiverit != receiver.end())
    {
        ss->reply(receiverit + " " + res);
        receiverit++;
    }
    //ss->reply("<nick> :<away message>";

}
