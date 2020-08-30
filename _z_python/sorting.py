#! python

# readonly data. do not edit.  [ "name", index, position, special_id]
mdata =[
	["EMPTY_C", 999999, 0, 874],
	["Potion x5", 200012, 1, 875],
	["Potion x20", 200012, 2, 556],
	["Magicgong", 200014, 3, 235],
	["Ether", 200015, 4, 587],
	["Elixir", 200016, 5, 547],
	["Megalixir", 200017, 6, 897],
	["Enfuss", 200019, 7, 652],
	["EMPTY_A", 999999, 8, 874],
	["EMPTY_B", 999999, 9, 214],
	["Lunar", 200010, 10, 542],
	["Lunar X", 200009, 11, 5421]
]
mdatacopy = [[i for i in a] for a in mdata ]

# sorting. editing is allowed.
listTC = []
tosort = 0
for f in mdata:
	if f[0][0:5] != "EMPTY":
		listTC.append(f)
		tosort += 0
#
bsorted = False

while bsorted == False:
	bsorted = True;
	for i in range(len(listTC) - 1):
		if listTC[i][1] > listTC[i+1][1]:
			bsorted = False;
			listTC[i],listTC[i+1] = listTC[i+1],listTC[i]
#
def finditembyIDandPOS( x, y, z):
	for data in z:
		if data[1] == x and data[2] == y:
			return data

def finditembyPOS( y,z):
	for data in z:
		if data[2] == y:
			return data

def finditembyID( x,z):
	for data in z:
		if data[1] == x:
			return data

def finditembySPECID( v,z):
	for data in z:
		if data[3] == v:
			return data
#
list_move = []
for i in range(len(listTC)):
	dataA = listTC[i]
	dataB = finditembySPECID(dataA[3],mdatacopy)
	dataC = mdatacopy[i]
	if dataA[2] != dataC[2]:
		indexA = mdatacopy.index(dataB);
		indexB = mdatacopy.index(dataC)
		list_move.append("move %d <-> %d [%s <-> %s]" % (indexA,indexB,dataB[0],dataC[0]   ) )
		#dataX = dataB
		#dataB = dataC
		#dataC = dataX
		mdatacopy[ indexA], mdatacopy[ indexB] = mdatacopy[ indexB], mdatacopy[ indexA ];
		
#printing
print( listTC )
print( "@@" )
print(mdata)
print( "@@" )
print(mdatacopy)
print( "@@" )

for cc in list_move:
	print(cc)





