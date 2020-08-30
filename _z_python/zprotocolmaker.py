#! python

import struct , binascii
#protocolfile = r"E:\\codeliteproj\\ZettaBeamX\\[CC]Header\\Protocol.h"
protocolfile = r"E:\\luna\\Source\\[CC]Header\\Protocol.h"
zprotocol = "zprotocol.dat"
dwversion = 8

class MyClass(object):
	_slot_ = ("cata","proto")
	def __init__(self):
		self.cata = ""
		self.proto = []


infile = open( protocolfile, "r")
skipable = False
worldlist = []
currentworld = None
for lines in infile:
	aline = lines.strip()
	aline = aline.replace("\r","")
	aline = aline.replace("\n","")
	aline = aline.replace("\t"," ")
	for n in range(10):
		aline = aline.replace("  "," ")
	#aline = lines.strip()
	if aline == "":
		continue
	if aline[0] == "{" or aline[0] == "}" or aline[0] == "#":
		if aline[0] == "}" and currentworld != None:
			worldlist.append( currentworld )
			currentworld = None
		continue
	if aline[0:2] == "//":
		continue
	if skipable:
		if aline.rfind( "*/") == -1:
			continue
		else:
			skipable = False
			continue
	if aline == "":
		continue
	if aline[:2] == "/*":
		if aline.rfind( "*/") == -1:
			skipable = True
		else:
			skipable = False
	if "enum" in aline:
		if currentworld != None:
			worldlist.append( currentworld )
		currentworld = MyClass()
		nme = aline.rsplit(" ")
		currentworld.cata = nme[1]
		continue
	if currentworld != None and "enum" not in aline[:8] :
		nyaa = []
		nyaa.append(aline.find(","))
		nyaa.append(aline.find(" "))
		nyaa.append(aline.find(" "))
		nyaa.append(aline.find(" "))
		bb = 0
		aa = len(aline)
		for vv in nyaa:
			if vv != -1 and vv < aa:
				aa = vv
				bb = aa
		if bb == 0:	
			currentworld.proto.append(aline)
		else:
			currentworld.proto.append(aline[:bb])
infile.close()
if currentworld != None:
			worldlist.append( currentworld )

#
def findin(str):
	for xc in worldlist:
		if str == xc.cata:
			return xc
	return None

bufferme = b''
idx = 0
worldcup = []
ourclass = MyClass()
ourclass.cata = "00"
worldcup.append( ourclass )
nyao = findin("MP_CATEGORY")
lvaluee = len(nyao.proto)

for cc in nyao.proto:
	asdd = ""
	if cc == "MP_RMTOOL_CONNECT":
		asdd = "MP_TOOLPROC_CONNECT"

	elif cc == "MP_SIEGERECALL":
		asdd = "MP_SIEGERECALL"
	elif cc == "MP_RECALLNPC":
		asdd = "MP_RECALLNPC"
	elif cc == "MP_PUNISH":
		asdd = "MP_PROTOCAOL_PUNISH"
	elif cc == "MP_HOUSE":
		asdd = "MP_PROTOCOL_HOUSING"
	elif cc == "MP_GUILD_WAR":
		asdd = "MP_PROTOCOL_GUILD_FIELDWAR"
	elif cc == "MP_STREETSTALL":
		asdd = "MP_PROTOCOL_STREESTALL"
	else:
		mew = cc.split( "_", 1)
		if len(mew) > 1:
			asdd = "%s_PROTOCOL_%s" % (mew[0], mew[1])
		else:
			asdd = cc
	hisa = findin( asdd )
	if hisa != None:
		hisa.cata = cc
		worldcup.append( hisa )
	else:
		ourclass = MyClass()
		ourclass.cata = cc
		worldcup.append( ourclass )
		print( "NOT FOUND",asdd )
for xclass in worldcup:
	#print( xclass.cata )
	strcound = bytearray(xclass.cata, "utf-8") + b"\x00"
	nn = len(strcound) % 4
	if nn != 0:
		strcound += ( b"\x00" * (4 - nn))
	tbuff = struct.pack( "<iii",idx,len(xclass.proto), len(strcound) )
	bufferme += ( tbuff + strcound )
	aa = 0
	for protocl in xclass.proto:
		#print( "   %s" % (protocl))
		strproto = bytearray(protocl, "utf-8") + b"\x00"
		mm = len(strproto) % 4
		if mm != 0:
			strproto += ( b"\x00" * (4 - mm))
		bufferme += ( struct.pack( "<i", len(strproto))  + strproto )
	idx += 1
#'''
outfile = open( zprotocol, "wb")
outfile.write(bytearray("ULTI","utf-8"))
outfile.write( struct.pack( "<III",dwversion,len(bufferme), len(worldcup) )  )
outfile.write(bufferme)

outfile.close()
#'''
print( "__DONE__")