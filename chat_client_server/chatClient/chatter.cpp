/*
 * chatter.cpp
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 */
#include "chatter.h"

chatter::chatter(int id)
{
    setUpChatClient(id);

}

chatter::~chatter()
{

}

void chatter::setUpChatClient(int id)
{
    // creates a new instance of chat client GUI
    chatty = new chatClient(id);
    chatty->show();
}
