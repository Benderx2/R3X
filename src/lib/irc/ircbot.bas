native(host_connect, "./host.so", 2)
native(host_send, "./host.so", 2)

function main(0)
	print "irc bot: sending stuff to ##ingsoc"
	let sockfd = @host_connect("irc.freenode.net", 6667)
	print "socket: "; sockfd
	@host_send(sockfd, "NICK rxvm_bot")
	@host_send(sockfd, "USER rxvm_bot 0 0 : rxvm_bot")
	@host_send(sockfd, "JOIN ##ingsoc")
	@host_send(sockfd, "PRIVMSG ##ingsoc bender|:testing.host_connect.")
	/* Todo: get proper vm version than using fixed ones, but fuck off */
	@host_send(sockfd, "PRIVMSG ##ingsoc Running.on.VM.Version:.0.4.7b")
	@host_send(sockfd, "PRIVMSG ##ingsoc Bye.Bye!")
	@host_send(sockfd, "QUIT")
	end
endf
