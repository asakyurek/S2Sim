/*
 * main.cpp
 *
 *  Created on: Sep 19, 2013
 *      Author: Alper
 */

#include <iostream>
#include <map>
#include <time.h>

#include "TCPServer.h"
#include "MatlabManager.h"
#include "ConnectionManager.h"
#include "ControlManager.h"

/*typedef std::map< ThreadedTCPConnectedClient*, int > TClientMap;
 
 TClientMap myMap;
 int number = 0;
 
 void
 BufferProcessor( ThreadedTCPConnectedClient* client, void* buffer, int size )
 {
 char* currentPointer = ( char* )buffer;
 unsigned int startOfMessage;
 unsigned short senderId;
 unsigned short receiverId;
 unsigned int sequenceNumber;
 unsigned short messageType;
 unsigned short messageId;
 unsigned int dataSize;
 unsigned int endOfMessage;
 
 memcpy( &startOfMessage, currentPointer, sizeof( startOfMessage ) );
 currentPointer += sizeof( startOfMessage );
 
 if ( 0x12345678 != ntohl( startOfMessage ) )
 {
 std::cout << "SOM Error" << std::endl;
 }
 
 memcpy( &senderId, currentPointer, sizeof( senderId ) );
 currentPointer += sizeof( senderId );
 
 std::cout << "Sender: " << ntohs( senderId ) << std::endl;
 
 memcpy( &receiverId, currentPointer, sizeof( receiverId ) );
 currentPointer += sizeof( receiverId );
 
 if ( 0x0000 != ntohs( receiverId ) )
 {
 std::cout << "Receiver ID Error " << ntohs( receiverId ) << std::endl;
 }
 
 memcpy( &sequenceNumber, currentPointer, sizeof( sequenceNumber ) );
 currentPointer += sizeof( sequenceNumber );
 
 std::cout << "Seq#: " << ntohl( sequenceNumber ) << std::endl;
 
 memcpy( &messageType, currentPointer, sizeof( messageType ) );
 currentPointer += sizeof( messageType );
 
 std::cout << "Type: " << ntohs( messageType ) << std::endl;
 
 memcpy( &messageId, currentPointer, sizeof( messageId ) );
 currentPointer += sizeof( messageId );
 
 std::cout << "Id: " << ntohs( messageId ) << std::endl;
 
 memcpy( &dataSize, currentPointer, sizeof( dataSize ) );
 currentPointer += sizeof( dataSize );
 
 dataSize = ntohl( dataSize );
 std::cout << "Data Size: " << dataSize << std::endl;
 
 char* objectName = new char[dataSize];
 
 memcpy( objectName, currentPointer, dataSize );
 currentPointer += dataSize;
 
 std::string receivedName = objectName;
 delete[] objectName;
 
 std::cout << "Name: " << receivedName << std::endl;
 
 memcpy( &endOfMessage, currentPointer, sizeof( endOfMessage ) );
 currentPointer += sizeof( endOfMessage );
 
 if ( 0xFEDCBA98 != ntohl( endOfMessage ) )
 {
 std::cout << "EOM Error" << std::endl;
 }
 
 if ( receivedName == "EBU2_NORTH" )
 {
 char* responseMessage = new char[36];
 currentPointer = responseMessage;
 startOfMessage = htonl( 0x12345678 );
 endOfMessage = htonl( 0xFEDCBA98 );
 
 memcpy( currentPointer, &startOfMessage, sizeof( startOfMessage ) );
 currentPointer += sizeof( startOfMessage );
 
 senderId = receiverId;
 receiverId = htons(number);
 messageId = htons(0x0002);
 dataSize = htonl(12);
 
 int requestResult = htonl(0);
 int systemTime = htonl( 123 );
 short clientNumber = htons( number + 1 );
 short systemMode = htons( 2 );
 
 memcpy( currentPointer, &senderId, sizeof( senderId ) );
 currentPointer += sizeof( senderId );
 
 memcpy( currentPointer, &receiverId, sizeof( receiverId ) );
 currentPointer += sizeof( receiverId );
 
 memcpy( currentPointer, &sequenceNumber, sizeof( sequenceNumber ) );
 currentPointer += sizeof( sequenceNumber );
 
 memcpy( currentPointer, &messageType, sizeof( messageType ) );
 currentPointer += sizeof( messageType );
 
 memcpy( currentPointer, &messageId, sizeof( messageId ) );
 currentPointer += sizeof( messageId );
 
 memcpy( currentPointer, &dataSize, sizeof( dataSize ) );
 currentPointer += sizeof( dataSize );
 
 memcpy( currentPointer, &requestResult, sizeof( requestResult ) );
 currentPointer += sizeof( requestResult );
 
 memcpy( currentPointer, &systemTime, sizeof( systemTime ) );
 currentPointer += sizeof( systemTime );
 
 memcpy( currentPointer, &clientNumber, sizeof( clientNumber ) );
 currentPointer += sizeof( clientNumber );
 
 memcpy( currentPointer, &systemMode, sizeof( systemMode ) );
 currentPointer += sizeof( systemMode );
 
 memcpy( currentPointer, &endOfMessage, sizeof( endOfMessage ) );
 currentPointer += sizeof( endOfMessage );
 
 std::cout << "Sent: " << client->SendData( responseMessage, 36 ) << std::endl;
 delete[] responseMessage;
 }
 }
 
 void
 ReceiveHandler( ThreadedTCPConnectedClient* client, void* buffer, int size )
 {
 if ( size <= 0 )
 {
 std::cout << myMap[client] << " deleted" << std::endl;
 delete client;
 }
 else
 {
 std::cout << myMap[client] << " Received a size: " << size << std::endl;
 BufferProcessor( client, buffer, size );
 }
 }
 
 void
 NotificationHandler( ThreadedTCPConnectedClient* acceptedClient )
 {
 ThreadedTCPConnectedClient* newClient = new ThreadedTCPConnectedClient( *acceptedClient );
 delete acceptedClient;
 newClient->SetNotificationCallback( &ReceiveHandler );
 myMap.insert( std::make_pair( newClient, number ) );
 ++number;
 }*/

int
main(int argc, char **argv)
{
    LOG_FUNCTION_START();
    LogPrint( "S2Sim Started" );
    
    GetConnectionManager();
    GetMatlabManager();
    GetControlManager();
    GetSystemManager();
    
    int dummy = 0;
    
    GetControlManager().WaitUntilReady();
    //int starting;
    //std::cin >> starting;
    while ( 1 )
    {
        //usleep(1);
        GetSystemManager().AdvanceTimeStep();
        ++dummy;
        LogPrint( "Time: ", dummy );
    }
    
    //std::cout << GetMatlabManager().IsClientPresent( "north_ebu2" ) << std::endl;
    //std::cin >> dummy;
    //std::cout << GetMatlabManager().IsClientPresent( "north_ebu23" ) << std::endl;
    //std::cin >> dummy;
    //GetMatlabManager().SetWattage( "north_ebu2", 123000 );
    
    //GetMatlabManager().AdvanceTimeStep();
    //std::cout << GetMatlabManager().GetWattage( "north_ebu2" ) << std::endl;
    //std::cout << GetMatlabManager().GetWattage( "north_ebu2" ) << std::endl;
    LOG_FUNCTION_END();
}



