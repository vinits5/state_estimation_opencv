all:
	g++ -ggdb marker_detection.cpp colorDetection.h colorDetection.cpp shapeDetection.h shapeDetection.cpp markerDetection.h markerDetection.cpp PoseEstimation.h PoseEstimation.cpp -o marker_detection `pkg-config --cflags --libs opencv`

clean:
	rm -rf *o marker_detection
