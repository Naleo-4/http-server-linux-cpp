#(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
#(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
#(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &

#curl -v --data "12345" -H "Content-Type: application/octet-stream" http://localhost:4221/files/file_123
#curl -v http://localhost:4221/echo/pineapple
#curl -v http://localhost:4221/files/temp
#curl -v http://localhost:4221/
#curl -v http://localhost:4221/banana
#curl -v http://localhost:4221
#curl -v --header "User-Agent: foobar/1.2.3" http://localhost:4221/user-agent
#curl -v http://localhost:4221/user-agent -H "User-Agent: banana/blueberry-strawberry"
#curl -v http://localhost:4221/echo/orange -H "Accept-Encoding: encoding-1, gzip, encoding-2"
curl -v -H "Accept-Encoding: gzip" http://localhost:4221/echo/abc | hexdump -C