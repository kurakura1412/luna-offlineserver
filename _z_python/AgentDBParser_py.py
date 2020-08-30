#! python

print( "____START____")

f1 = open("E:/luna/Source/[Server]Agent/AgentDBMsgParser.cpp", "rb")
flog = open("AgentDBMsgParser.cpp", "wb")
bufe = f1.read()
f1.close()

line33 = bufe.split(b'\x0D\x0A')
fr = len(line33)
lseek = 0
while lseek < fr:
	lin3 = line33[lseek]
	flog.write(lin3 + b'\x0D\x0A')
	
	lseek += 1
	


flog.close()
print( "____END_____")







