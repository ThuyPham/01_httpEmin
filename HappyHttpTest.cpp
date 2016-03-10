/* Raspberry Pi run as a Client 
   ThingSpeak run as a Web Server
*/

#include <stdio.h>
#include <strings.h>
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

	conn.request( "GET", "https://api.thingspeak.com/update?api_key=182FE3HT431N53KM&field1=24", 0, 0,0 );

	while( conn.outstanding() )
		conn.pump();

}
