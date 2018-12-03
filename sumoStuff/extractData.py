f=open('data.xml','r')
tsteps=0
carsteps=0
for l in f.readlines():
	i=l.find('running=')
	if i<0 or i>=len(l):
		continue
	i+=9
	v=0
	while ord(l[i])<=ord('9') and ord(l[i])>=ord('0'):
		v*=10
		v+=int(l[i])
		#print(v)
		
		i+=1
	tsteps+=1
	carsteps+=v
	
cavg=carsteps/tsteps
print('Steps:' + str(tsteps))
print('Car Steps:' + str(carsteps))
print('C_avg:'+str(cavg))
