def leng_model():
	f= open('p1.txt','r')

	a = f.read()
	tk = a.split()
	i=0
	dic = {}

	for wd in tk:
		dic[wd]= 0
	for wd in tk:
		dic[wd]+=1

	print(dic)
