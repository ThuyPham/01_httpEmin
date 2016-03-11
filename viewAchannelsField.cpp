/* Raspberry Pi run as a Client 
   ThingSpeak run as a Web Server

   Get a Channel Feed - View a channel feed
*/

#include <stdio.h>
#include <string.h>
#include "happyhttp.h"

int count=0;

void OnBegin( const happyhttp::Response* r, void* userdata )
{
	printf( "BEGIN (%d %s)\n", r->getstatus(), r->getreason() );
	count = 0;
}

void OnData( const happyhttp::Response* r, void* userdata, const unsigned char* data, int n )
{
	fwrite( data,1,n, stdout );
	count += n;
}

void OnComplete( const happyhttp::Response* r, void* userdata )
{
	printf( "COMPLETE (%d bytes)\n", count );
}

int main(int argc, char* argv[]){
	happyhttp::Connection conn( "thingspeak.com", 80 );
	conn.setcallbacks( OnBegin, OnData, OnComplete, 0 );

	conn.request( "GET", "https://api.thingspeak.com/channels/62889/fields/1.json", 0, 0,0 );

	while( conn.outstanding() )
		conn.pump();
}