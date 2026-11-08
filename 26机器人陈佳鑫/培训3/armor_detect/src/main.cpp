#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    VideoCapture cap("video/armor.mp4");
    int w = cap.get(CAP_PROP_FRAME_WIDTH);
    int h = cap.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter writer("results/armor_result.mp4",
        VideoWriter::fourcc('m', 'p', '4', 'v'), 30, Size(w, h));

    Mat frame;
    while (cap.read(frame))
    {
        Mat hsv, m1, m2, mask;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        inRange(hsv, Scalar(0, 80, 80), Scalar(10, 255, 255), m1);
        inRange(hsv, Scalar(170, 80, 80), Scalar(179, 255, 255), m2);
        mask = m1 + m2;

        Mat labels, stats, centroids;
        int n = connectedComponentsWithStats(mask, labels, stats, centroids);

        int i1 = 0, i2 = 0, a1 = 0, a2 = 0;
        for (int i = 1; i < n; i++)
        {
            int a = stats.at<int>(i, CC_STAT_AREA);
            if (a > a1) { a2 = a1; i2 = i1; a1 = a; i1 = i; }
            else if (a > a2) { a2 = a; i2 = i; }
        }

        if (i2 != 0)
        {
            Rect r1(stats.at<int>(i1, CC_STAT_LEFT), stats.at<int>(i1, CC_STAT_TOP),
                    stats.at<int>(i1, CC_STAT_WIDTH), stats.at<int>(i1, CC_STAT_HEIGHT));
            Rect r2(stats.at<int>(i2, CC_STAT_LEFT), stats.at<int>(i2, CC_STAT_TOP),
                    stats.at<int>(i2, CC_STAT_WIDTH), stats.at<int>(i2, CC_STAT_HEIGHT));
            if (r1.x > r2.x)
            {
                Rect tr = r1; r1 = r2; r2 = tr;
                int ti = i1; i1 = i2; i2 = ti;
            }
            rectangle(frame, r1, Scalar(0, 255, 255), 2);
            rectangle(frame, r2, Scalar(0, 255, 255), 2);

            int id[2] = { i1, i2 };
            Point top[2], bot[2];
            for (int k = 0; k < 2; k++)
            {
                int bx = stats.at<int>(id[k], CC_STAT_LEFT);
                int by = stats.at<int>(id[k], CC_STAT_TOP);
                int bw = stats.at<int>(id[k], CC_STAT_WIDTH);
                int bh = stats.at<int>(id[k], CC_STAT_HEIGHT);

                Mat pts;
                for (int y = by; y < by + bh; y++)
                    for (int x = bx; x < bx + bw; x++)
                        if (labels.at<int>(y, x) == id[k])
                            pts.push_back(Point(x, y));

                RotatedRect rr = minAreaRect(pts);
                Point2f fp[4];
                rr.points(fp);

                double l01 = norm(fp[1] - fp[0]);
                double l12 = norm(fp[2] - fp[1]);
                int a0 = 0, a1 = 1, b0 = 2, b1 = 3;
                if (l12 > l01)
                {
                    a0 = 1; a1 = 2;
                    b0 = 3; b1 = 0;
                }
                int ea = a0, eb = a1;
                if (k == 0 && fp[b0].x + fp[b1].x < fp[a0].x + fp[a1].x) { ea = b0; eb = b1; }
                if (k == 1 && fp[b0].x + fp[b1].x > fp[a0].x + fp[a1].x) { ea = b0; eb = b1; }
                Point2f pa = fp[ea], pb = fp[eb];
                if (pa.y > pb.y) { Point2f t = pa; pa = pb; pb = t; }
                top[k] = Point(pa.x, pa.y);
                bot[k] = Point(pb.x, pb.y);
            }

            Point p[4] = { top[0], top[1], bot[1], bot[0] };
            for (int i = 0; i < 4; i++)
                line(frame, p[i], p[(i + 1) % 4], Scalar(0, 255, 0), 2);
            Point c((p[0].x + p[1].x + p[2].x + p[3].x) / 4,
                    (p[0].y + p[1].y + p[2].y + p[3].y) / 4);
            line(frame, c + Point(-12, 0), c + Point(12, 0), Scalar(0, 255, 0), 2);
            line(frame, c + Point(0, -12), c + Point(0, 12), Scalar(0, 255, 0), 2);
        }

        imshow("result", frame);
        writer.write(frame);
        waitKey(1);
    }
    return 0;
}
