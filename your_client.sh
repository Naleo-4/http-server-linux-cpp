#(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
#(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
#(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &

#curl -v http://localhost:4221/echo/pineapple
curl -v http://localhost:4221/files/tem
#curl -v http://localhost:4221/
#curl -v http://localhost:4221
#curl -v --header "User-Agent: foobar/1.2.3" http://localhost:4221/user-agent