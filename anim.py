#!/usr/bin/python2.6
 
# -*-coding:Latin-1 -*

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.animation as animation
import sys


FPS = 10
x = 32


if len(sys.argv) > 1:
    fichier = sys.argv[1]
    img = mpimg.imread(fichier)
    if img.dtype == np.float32:
        img = (img * 255).astype(np.uint8)
    morceau_im = img[:,:x]
    fig = plt.figure(figsize=(5,5))
    plt.axis('off')
    im_anim = plt.imshow(morceau_im)

    def animate(i):
        im_anim.set_array(img[:,i*x:(i+1)*x])
        return im_anim,
    def init():
        im_anim.set_array(img[:,:x])
        return im_anim,
    animiii = animation.FuncAnimation(fig, animate, init_func=init, frames=len(img[0])/x, interval = 1./FPS*1000, repeat=True)
    plt.show()
else:
    print("Donne le nom du fichier en parametre encule")
    
