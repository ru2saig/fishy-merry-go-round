import argparse
import time
import os
import numpy as np
import cv2



def extract_fish(image): # TODO: Have to config this stuff each time
    """Extract and return image of the fish to save_location/(timestamp).png."""
    # image = cv2.imread("fish-1.png")  # capture from camera
    image = cv2.cvtColor(image, cv2.COLOR_BGR2BGRA)
    #image = cv2.resize(image, (int(image.shape[1]/2), int(image.shape[0]/2))) 
    image = image[50:(image.shape[0] - 130), 80:(image.shape[1] - 210)]  # extract the ROI 

    # conversion to an inverted image for better contour detection
    img_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    img_gray = cv2.bitwise_not(img_gray)

    # applying a basic threshold
    ret, thresh = cv2.threshold(img_gray, 190, 255, cv2.THRESH_BINARY)

    contours, hierarchy = cv2.findContours(image=thresh, mode=cv2.RETR_EXTERNAL, method=cv2.CHAIN_APPROX_SIMPLE)

    # extract the fish, from the contour
    mask = np.zeros(image.shape[:2], dtype=np.uint8)
    cv2.drawContours(image=mask, contours=contours, contourIdx=-1, color=(255, 255, 255, 255), thickness=cv2.FILLED)
    fish = cv2.bitwise_and(image, image, mask=mask)

    return fish


# CHANGME: to use a GPIO connected button instead
def is_button_pressed():
    input("Click enter")
    return True


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Client program that runs on the scannerbox.")
    parser.add_argument("--path", help="Directory to save the extracted fish file in. Defaults to the current directory.", default=os.getcwd())
    parser.add_argument("--cam", help="Camera index to use for capturing data.", type=int, default=0)
    args = parser.parse_args()

    cap = cv2.VideoCapture(args.cam)
    if not cap.isOpened():
        print("Cannot open camera")
        exit(1)

    while True:
        ret, frame = cap.read()

        img = extract_fish(frame)
        cv2.imshow("img", img)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('s'):
            file_path = os.path.join(args.path, str(int(time.time())) + ".png")
            cv2.imwrite(file_path, img)
            print(f"Saved image to {file_path}")

    cap.release()
    cv2.destroyAllWindows()

