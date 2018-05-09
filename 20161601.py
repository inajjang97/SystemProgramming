import requests
import re
from bs4 import BeautifulSoup
count=0
song=[]
f= open('URL.txt', 'w')
df = "http://cspro.sogang.ac.kr/~gr120170213/"
def ina(link):	
	global song
	link = re.sub(r"#","",link)
	link = re.sub(r"\?","",link)
	
	if link[0:7] != "http://":
		link1 = df+link
	else:
		link1 = link

	if link1 in song:
		return
	song.append(link1)
	r = requests.get(link1)
	soup = BeautifulSoup(r.content,"html.parser")
	results = soup.find_all('a')
	if r.ok == False:
		return
	if r.status_code == 404:
		return
	f.write(link1+"\n")
	
	global count
	count= count+1
	str1 = "Output_{:04}.txt".format(count)
	f2 = open(str1,"w")
	f2.write(soup.get_text())
	f2.close()

	for i in results :
		if i["href"] == "":
			continue
		ina(i["href"])
song.append("http://cspro.sogang.ac.kr/~gr120170213/index.html")
ina(df)
f.close()
