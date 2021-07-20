#include <iostream>
// 일치하는 정보 목록 반환
// <name> 매개 변수와 일치하는 정보 목록을 반환하는 쿼리를 생성하는 데 사용
std::string		Frame::cmdWho(Session *ss, std::vector<std::string> const& sets)
{
    // error 402, 352, 315
    // name 에 매개 변수가 없으면 표시되는 모든 사용자(보이지않는 사용자 모드 + i)와
    // 요청 클라이언트와 공통 채널이 없는 사용자)가 나열됨
    // 동일한 결과는 가능한 모든 항목과 일치하는 name 또는 와일드카드를 사용하여 얻음
    // channal name을 찾을 수 없는 경우 사용자의 호스트,서버,실명 및 닉네임과 일치
    // "o" 매개 변수가 전달되면 제공된 이름 마스크에 따라 연산자만 변환

    // 채널 유저호스트
    UserMap::iterator it;
    std::string res;
    std::string eol;


    // channel user host server nick realname
    if (sets.size() == 1)
    {
        while (it != it.end())
        {
            res += mUsers.nick();
            ss->reply();
            it++;
        }
        // 모든 유저 리스트 리턴
        
    }
    eol = sets[1] + " :End of /WHO list";
    if (sets[2] == "o") // 옵션이 있을 경우
    {
        // 관리자 리스트만 리턴
    }
    else
    {
        if (doesChannelExists(sets[1]))     // findChannel 사용?
        {

        }
        if (userhost.find(sets[1]) == userhost.end())
        {

        }
        if (server.find(sets[1]) == server.end())
            return ss->reply("402");   //NOSUCHSERVER
        if (mUsers.find(sets[1]) == mUsers.end())
        {

        }
        if (doesNicknameExists(sets[1]))
        {

        }
    }
    //RPL_WHOREPLY
    "<channel> <user> <host> <server> <nick> \ <H|G>[*][@|+] :<hopcount>(0) <real name>"

    return ss->reply(eol);
    //RPL_ENDOFWHO
    //"<name> :End of /WHO list"
}

std::string		Frame::cmdWhoi(Session *ss, std::vector<std::string> const& sets)
{
    // error 402 431 311 312 313 317 318 319 301 401
    if (server.find(set[1]) == server.end())
        return "402";   //NOSUCHSERVER
    if (mUsers.find(sets[0]) == mUsers.end())
        return "431";
    
}

std::string		Frame::cmdWhowas(Session *ss, std::vector<std::string> const& sets)
{
    // ERROR 369 431
    std::string res;
    if (sets.size() == 1 || )   // 닉네임 처음이 숫자 가능?
    {
        ss->reply("431");
    }
    if (mUsers.find(sets[0]) == mUsers.end())
        return "369";
    "<nick> :End of WHOWAS"

}

std::string		Frame::cmdPrivmsg(Session *ss, std::vector<std::string> const& sets)
{
    // ERROR 301 401 407 411 412 413 414 

    int i = 2;
    std::string res;
    iterator it = sets.begin();
    std::vector<std::string> receiver;
    iterator receiverit = receiver.begin();
    std::vector<std:string> msg;
    iterator msgit = msg.begin();

    if (sets[1][0] == ':')   // receiver가 없을 경우
    {
        return "411";   // ERR_NORECIPIENT
    }
    else if (sets[2][0] != ':')
    {
        return "412";   // ERR_NOSUCHSERVER
    }
    // receiver에 콤마로 split해서 저장하기


    // 메세지 전까지 확인하기
    while (receiverit != receiver.end())
    {
        if (nickname.find(sets[i]) == nickname.end())   // nickname이 어디에 들어있나유 mUser->sNickname??
            return "401";       // 401 : 닉네임
        if (mChannels.find(sets[i]) == mChannels.end())
            return "404";   // 404 : 채널이 없음

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
    //return "<nick> :<away message>";

}
