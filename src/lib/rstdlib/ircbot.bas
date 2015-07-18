#include <rstdlib.h>
#define HOST_READ_MAXLINE 512
function main(0)
	let buf = alloc(HOST_READ_MAXLINE+1)
	print "irc bot: sending stuff to ##ingsoc @ irc.freenode.net"
	let sockfd = @r_connect("irc.freenode.net", 6667)
	print "socket: "; sockfd
	@r_send(sockfd, "NICK rxvm_bot")
	@r_send(sockfd, "USER rxvm_bot 0 0 : rxvm_bot")
	@r_send(sockfd, "JOIN ##ingsoc")
	@r_send(sockfd, "PRIVMSG ##ingsoc bender|:testing.host_connect......")
	/* Todo: get proper vm version than using fixed ones, but fuck off */
	@r_send(sockfd, "PRIVMSG ##ingsoc Running.on.VM.Version:.0.4.7b")
	@r_send(sockfd, "PRIVMSG ##ingsoc Bye.Bye!")
	print "sent the stuff, now receiving, press a key to quit"
	let a = 0
	while (a := 0)
		@r_receive(sockfd, HOST_READ_MAXLINE, buf)
		print "->"; $buf 
	endw
	@r_send(sockfd, "QUIT")
	end
endf
