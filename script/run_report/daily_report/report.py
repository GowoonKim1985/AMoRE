
import numpy as np
#import matplotlib.pyplot as plt
#from matplotlib import pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

import os
import skimage
import IPython.display
from PIL import Image
import numpy as np
import cv2
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

from datetime import datetime

now = datetime.now()
filetime = now.strftime("%y%m%d_%H-%M-%S")
filename = f"./pdf/{filetime}.pdf"

#pdf 생성

#mypdf = PdfPages('report.pdf')
mypdf = PdfPages(filename)
#pedimg = []

#fig,axs = plt.subplots(4,2, figsize=(6,8))
fig,axs = plt.subplots(3,2, figsize=(6,8))
twimg1 = plt.imread('./figure/wcmd_rate.png')
twimg2 = plt.imread('./figure/wcmd_qsum.png')
twimg3 = plt.imread('./figure/wcmd_chbit.png')
twimg4 = plt.imread('./figure/wcmd_mul.png')
twimg5 = plt.imread('./figure/wcmd_hmap1.png')
twimg6 = plt.imread('./figure/wcmd_hmap2.png')

tpimg1 = plt.imread('./figure/psmd1_rate.png')
tpimg2 = plt.imread('./figure/psmd2_rate.png')
tpimg3 = plt.imread('./figure/psmd_hmap.png')


'''
plt.imread(twimg1)
plt.imread(twimg2)
plt.imread(twimg3)
plt.imread(twimg4)
plt.imread(twimg5)
plt.imread(twimg6)
'''
axs[0,0].imshow(twimg1)
axs[0,0].axis("off")
axs[0,1].imshow(twimg2)
axs[0,1].axis("off")
axs[1,0].imshow(twimg3)
axs[1,0].axis("off")
axs[1,1].imshow(twimg4)
axs[1,1].axis("off")
axs[2,0].imshow(twimg5)
axs[2,0].axis("off")
axs[2,1].imshow(twimg6)
axs[2,1].axis("off")
'''
axs[3,0].imshow(tpimg1)
axs[3,0].axis("off")
axs[3,1].imshow(tpimg2)
axs[3,1].axis("off")
'''     
#plt.show()

subrun_info = 't_rate.txt'
with open(subrun_info, 'r') as txtfile:
#    second_line = txtfile.readlines()[1]
    lines = txtfile.readlines()
    runnum = int(lines[0].strip())
    subrun = int(lines[1].strip()) -1
#    subrun = subrun-1
#print (subrun)

#plt.suptitle('Current status :subrun '+str(subrun), fontsize=16)
plt.suptitle(f'Current status : run ({runnum}) / subrun ({subrun})', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)


fig,axs = plt.subplots(4,2,figsize=(6,8))

axs[0,0].imshow(tpimg1)
axs[0,0].axis("off")
axs[0,1].imshow(tpimg2)
axs[0,1].axis("off")

for i in range(1,4):
    for j in range(0,2):
        axs[i,j].remove()

big_axs = fig.add_subplot(4,1,(2,4))    
big_axs.imshow(tpimg3)
big_axs.axis("off")

#axs.imshow(tpimg3)



#axs.axis('off')
mypdf.savefig(fig, dpi=350)




fig,axs = plt.subplots(4,3, figsize=(6,8))

for wch in range(1,49):
    pedimg_name = './figure/wcmd_ped_time_'+str(wch)+'.png'
#    qtotimg_name = './figure/wcmd_qtot_'+str(wch)+'.png'
    globals()['pedimg'+str(wch)] = plt.imread(pedimg_name)


fig,axs = plt.subplots(4,3, figsize=(6,8))

chnum=1
for i in range(0,4):
    for j in range(0,3):
        axs[i,j].imshow(globals()['pedimg'+str(chnum)])
        axs[i,j].axis("off")
        chnum=chnum+1

#plt.show()
plt.suptitle('WCMD [ped mean 1~12]', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)

for i in range(0,4):
    for j in range(0,3):
        axs[i,j].imshow(globals()['pedimg'+str(chnum)])
        axs[i,j].axis("off")
        chnum=chnum+1

#plt.show()
plt.suptitle('WCMD [ped mean 13~24]', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)

for i in range(0,4):
    for j in range(0,3):
        axs[i,j].imshow(globals()['pedimg'+str(chnum)])
        axs[i,j].axis("off")
        chnum=chnum+1

#plt.show()
plt.suptitle('WCMD [ped mean 25~36]', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)

for i in range(0,4):
    for j in range(0,3):
        axs[i,j].imshow(globals()['pedimg'+str(chnum)])
        axs[i,j].axis("off")
        chnum=chnum+1

#plt.show()
plt.suptitle('WCMD [ped mean 37~48]', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)


qtotimg_name = './figure/wcmd_qtot.png'
qtotimg = plt.imread(qtotimg_name)
plt.figure(figsize=(10,10))
plt.imshow(qtotimg)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.axis('off')
mypdf.savefig(dpi=350)

#    globals()['qtotimg'+str(wch)] = plt.imread(qtotimg_name)    


'''
#qtot draw
chnum=1
for i in range(0,4):
    for j in range(0,3):
        axs[i,j].imshow(globals()['qtotimg'+str(chnum)])
        axs[i,j].axis("off")
        chnum=chnum+1

#plt.show()
plt.suptitle('WCMD [Qtot 1~12]', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)

for i in range(0,4):
    for j in range(0,3):
        axs[i,j].imshow(globals()['qtotimg'+str(chnum)])
        axs[i,j].axis("off")
        chnum=chnum+1

#plt.show()
plt.suptitle('WCMD [Qtot 13~24]', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)

for i in range(0,4):
    for j in range(0,3):
        axs[i,j].imshow(globals()['qtotimg'+str(chnum)])
        axs[i,j].axis("off")
        chnum=chnum+1

#plt.show()
plt.suptitle('WCMD [Qtot 25~36]', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)

for i in range(0,4):
    for j in range(0,3):
        axs[i,j].imshow(globals()['qtotimg'+str(chnum)])
        axs[i,j].axis("off")
        chnum=chnum+1

#plt.show()
plt.suptitle('WCMD [Qtot 37~48]', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
mypdf.savefig(fig, dpi=350)

'''
'''
#page1

fig = plt.figure(figsize=(5, 10), dpi=100)

plt.hist(data[0],color='red')

mypdf.savefig(fig)


#page2

fig = plt.figure(figsize=(5, 10), dpi=100)

plt.hist(data[1],color='blue')

mypdf.savefig(fig)


#page3

fig = plt.figure(figsize=(5, 10), dpi=100)

plt.hist(data[2],color='green')

mypdf.savefig(fig)

'''
#저장

mypdf.close()
