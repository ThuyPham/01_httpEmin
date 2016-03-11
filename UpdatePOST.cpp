/* Raspberry Pi run as a Client 
   ThingSpeak run as a Web Server

   Update Channel Feed - POST
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
	const char* params = "api_key=182FE3HT431N53KM&field1=73";
	int l = strlen(params);

	happyhttp::Connection conn( "thingspeak.com", 80 );
	conn.setcallbacks( OnBegin, OnData, OnComplete, 0 );

	conn.putrequest( "POST", "https://api.thingspeak.com/update.json" );
	conn.putheader( "Connection", "close" );
	conn.putheader( "Content-Length", l );
	conn.putheader( "Content-type", "application/x-www-form-urlencoded" );
	conn.putheader( "Accept", "text/plain" );
	conn.endheaders();
	conn.send( (const unsigned char*)params, l );

	while( conn.outstanding() )
		conn.pump();
}