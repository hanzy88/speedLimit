# speedLimit

The speedLimit is mainly based on the matchTemplate of opencv, which using the houghCircle for roi.

And First, you should use the speedLimit::saveTem function to save the template of traffic signs picture as .txt file. Then, the input sign will find the best match of the saved template with speedLimit::recognizeDigit function.

For detail, you can check the main.cpp.

Actually, this repos is for the recognization of speed Limit traffic sign with objection detection like ssd, centerNet etc. For the better performance, you can improve the related function.
