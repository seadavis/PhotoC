import sys
from os import listdir, path
import cv2
import math

'''
This script will subtract a mask and then save the file with
a transparent background
Takes in five arguments. First argument is the folder containg 
the source images, second argument is the folder of the mask,
 third argument is where to dump the results of the extraction, fourth argument 
 is the folder to gind the target_images and fifth argument is where we
 want to dump the naive composites

The second argument is the foler containing the masks.
Each mask image must be named xmask.png where x is the name of
the original image,

For example. Say we had a picture called monkey.png, then
the mask to subract would be called monkeymask.png

Dumps the result into the third arguments
'''
def main():
    src_dir = sys.argv[1]
    mask_dir = sys.argv[2]
    dump_dir = sys.argv[3]
    tgt_dir = sys.argv[4]
    naive_dir = sys.argv[5]

    print(f"Source Dir: {sys.argv[1]}, Mask Dir: {sys.argv[2]}")

    src_files = listdir(src_dir)
    mask_files = listdir(mask_dir)
    tgt_files = listdir(tgt_dir)

    for src_file in src_files:
        matching_files = [f for f in mask_files if f"{path.splitext(src_file)[0]}mask" == path.splitext(f)[0]]
    
        if len(matching_files) == 0:
            print(f"Couldn't find file for: {src_file} moving on")

        mask_file = matching_files[0]

        print(f"Found Mask file: {mask_file} for Source file: {src_file}")

        src = cv2.imread(f"{src_dir}/{src_file}")

        #generate the mask and the inverse mask
        mask_color = cv2.imread(f"{mask_dir}/{mask_file}")
        mask_gray = cv2.cvtColor(mask_color, cv2.COLOR_BGR2GRAY)
        _,mask = cv2.threshold(mask_gray, 0, 255, cv2.THRESH_BINARY)
        mask_inv = cv2.bitwise_not(mask)

         # taking only the mask portion of the source image
        diff = cv2.bitwise_and(src,src,mask = mask)

        # converting the diff to an image with a transparent background
        tmp = cv2.cvtColor(diff, cv2.COLOR_BGR2GRAY)
        _,alpha = cv2.threshold(tmp,0,255,cv2.THRESH_BINARY)
        b, g, r = cv2.split(diff)
        rgba = [b,g,r, alpha]
        dst = cv2.merge(rgba,4)

        cv2.imwrite(f"{dump_dir}/{src_file}", dst)

        for tgt_file in tgt_files:
            print(f"Compositing Pictures Naively. Source: {src_file}, Target: {tgt_file}")
            tgt = cv2.imread(f"{tgt_dir}/{tgt_file}")
            src_w, src_h, channels = diff.shape
            tgt_w, tgt_h, channels = tgt.shape

            print(f"Src Width: {src_w}, Src H: {src_h}\n")
            print(f"Tgt Width: {tgt_w}, Tgt H: {tgt_h}\n")
        
            # figure out where we start and end to place
            # it in the middle
            start_row = math.floor(tgt_w/2) - math.floor(src_w/2)
            end_row = start_row + src_w

            start_col = math.floor(tgt_h/2) - math.floor(src_h/2)
            end_col = start_col + src_h

            # take the roi from the target
            roi = tgt[start_row:end_row, start_col:end_col]

            #blackout the area of the image
            roi_bg = cv2.bitwise_and(roi, roi, mask= mask_inv)
            
            # compose the roi with the source
            composed = cv2.add(roi_bg, diff)

            # put the roi into the target image
            tgt[start_row:end_row, start_col:end_col] = composed

            cv2.imwrite(f"{naive_dir}/{path.splitext(src_file)[0]}_{path.splitext(tgt_file)[0]}.png", tgt)


       
        
if __name__ == "__main__":
    main()