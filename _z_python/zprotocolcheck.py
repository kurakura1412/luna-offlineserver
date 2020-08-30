#! python

import struct , binascii

infile9 = "zprotocol.dat"
infile7 = r"E:\\codeliteproj\\ZettaBeamX\\libe\\bin\\zprotocol_old.dat"


class aclass(object):
	def __init__(self):
		self.cata = ""
		self.proto = []
		
def findin(str, alist):
	for xc in alist:
		if str == xc.cata:
			return xc
	return None

rakesa = ""
ricc = ""
def readat(instr, tolist):
	magic,version,msize,mcount = struct.unpack("<IIII", instr.read(16))
	for ii in range(mcount):
		ncla = aclass()
		ricc = ""
		tabuf = b''
		idx, zcount,strlens = struct.unpack("<iii", instr.read(12))
		try:
			tabuf = instr.read(strlens)
		except:
			print(strlens, instr.tell())
			exit(1)
		ricc = str(tabuf,encoding='utf-8')
		ricc = ricc.replace('\x00','')
		ncla.cata = ricc
		
		for xx in  range( zcount ):
			rakesa = ""
			strcnt = struct.unpack("<i", instr.read(4))[0]
			#instr.read(4)
			nabuf = instr.read(strcnt)
			rakesa = str(nabuf,encoding='utf-8') #.replace(b'\x00','')
			rakesa = rakesa.replace('\x00','')
			ncla.proto.append( rakesa )
			if rakesa == ricc:
				print( rakesa, ricc,nabuf,str(nabuf,encoding='utf-8'),nmaa,strcnt,hex(instr.tell())  )
				exit(0)
		tolist.append( ncla)
#

v9 = []
v7 = []
id9 = open(infile9, "rb")
readat(id9, v9)
id9.close()
id7 = open(infile7, "rb")
readat(id7, v7)
id7.close()

for mm in v7:
	vcc = findin( mm.cata, v9 )
	if vcc == None:
		print( "NOT_FOUND:", mm.cata )
		continue
	if len( mm.proto ) != len( vcc.proto ):
		print( "PROTO:", mm.cata )
		for prt in mm.proto:
			foundme =  False
			for trp in vcc.proto:
				if prt == trp:
					foundme = True
					break
			if foundme == False:
				print( "NOT_FOUND prt:", prt )

