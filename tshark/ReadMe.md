tshark -i enp0s31f6 -Y http.request -T fields -e http.host -e http.user_agent 

tshark -i enp0s31f6 -f "src port 53" -n -T fields -e dns.qry.name -e dns.resp.addr

tshark -i enp0s31f6 -Y "frame matches \"bilibili.com\"" 