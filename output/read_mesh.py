from pyMCDS import pyMCDS
import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

#mcds1 = pyMCDS('output00000002.xml')
#mcds1 = pyMCDS('output00000001.xml')
#mcds1 = pyMCDS('initial.xml')
mcds1 = pyMCDS('final.xml')
#mcds1 = pyMCDS('first_out.xml')

print(mcds1.get_time())
oxy = mcds1.get_concentrations('oxygen')
print('oxy.shape = ',oxy.shape)  # (100,100,100)
zslice = 50
oxy_z0 = oxy[:,:,zslice]   
print('oxy_z0.shape = ',oxy_z0.shape)  # (100,100)
print('min,max = ',oxy_z0.min(),oxy_z0.max())

fig = plt.figure(figsize=(7,5.8))
xvec = np.arange(-1000,1000,20)
num_contours = 30
my_plot = plt.contourf(xvec,xvec, oxy_z0, num_contours, cmap='viridis') #'viridis'
plt.colorbar(my_plot)
axes_min = -1000
axes_max = 1000
plt.xlim(axes_min,axes_max)
plt.ylim(axes_min,axes_max)
plt.title('oxygen at z=' + str(zslice))
#plt.axis('equal')
plt.show()
