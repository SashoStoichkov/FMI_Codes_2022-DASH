using System;
using System.IO;
using System.Drawing;
using LazZiya.ImageResize;
using System.Collections.Generic;

namespace ProcessingImage
{

    public class ImageProcess
    {
        static List<int[]> rubikColors = new List<int[]>()
        {
            new int[3] { 255,255,255 }, //White
            new int[3]  { 255, 255, 0 }, // Yellow
            new int[3] { 255, 0, 0 }, // Red
            new int[3]  { 0, 0, 255 }, // Blue
            new int[3]    { 255, 165, 0 }, // Orange
            new int[3]  { 0, 255, 0 }, // Green
        };
        static List<int[]> basicColors = new List<int[]>()
        {
            new int[3] { 255,255,255 },
            new int[3]  { 255, 255, 0 },
            new int[3] { 255, 0, 0 },
            new int[3]  { 0, 0, 255 },
            new int[3]    { 255, 165, 0 },
            new int[3]  { 0, 255, 0 },

            new int[3] { 255,255,255 },
            new int[3]  { 255, 255, 0 },
            new int[3] { 255, 0, 0 },
            new int[3]  { 0, 0, 255 },
            new int[3]    { 255, 165, 0 },
            new int[3]  { 0, 255, 0 },
        };

        //double[] whiteCoordinates = new double[3] { 255, 255, 255 };
        //double[] yellowCoordinates = new double[3] { 255, 255, 0 };
        //double[] redCoordinates = new double[3] { 255, 0, 0 };
        //double[] blueCoordinates = new double[3] { 0, 0, 255 };
        //double[] orangeCoordinates = new double[3] { 255, 165, 0 };
        //double[] greenCoordinates = new double[3] { 0, 255, 0 };

        static double getDistanceBetweenTwoPoints(double x1, double y1, double z1, double x2, double y2, double z2)
        {
            double dx = x1 - x2;
            double dy = y1 - y2;
            double dz = z1 - z2;

            return Math.Sqrt(dx * dx + dy * dy + dz * dz);
        }

        static void SaveToFile(Bitmap bitmap)
        {
            for (int i = 0; i < bitmap.Width; i++)
            {
                for (int j = 0; j < bitmap.Height; j++)
                {
                    var pixel = bitmap.GetPixel(i, j);
                    File.AppendAllText("image.txt", $"{pixel.R} {pixel.G} {pixel.B}{Environment.NewLine}");
                }
            }
        }

        public static void ProcessImage(int width, int height)
        {
            Bitmap bitmap = null;
            Bitmap cleared = null;
            try
            {

                Scale(width, height);
                bitmap = new Bitmap(resizedOutput);
                cleared = ClearImage(bitmap);
                bitmap = Pixilise(bitmap);
                bitmap.Save(pixilised);
            }
            finally
            {
                bitmap.Dispose();
                cleared.Dispose();
            }

        }

        static Bitmap Pixilise(Bitmap bitmap)
        {
            for (int i = 0; i < bitmap.Width; i++)
            {
                for (int j = 0; j < bitmap.Height; j++)
                {
                    double minDistance = int.MaxValue;

                    Color colorCurrent = bitmap.GetPixel(i, j);

                    foreach (var point in rubikColors)
                    {
                        double currentDistance = getDistanceBetweenTwoPoints(colorCurrent.R, colorCurrent.G, colorCurrent.B, point[0], point[1], point[2]);
                        if (currentDistance < minDistance)
                        {
                            minDistance = currentDistance;
                            Color newColor = Color.FromArgb(point[0], point[1], point[2]);
                            bitmap.SetPixel(i, j, newColor);
                        }
                    }
                }
            }

            return bitmap;
            //bitmap.Save(@"C:\Users\user 1\Desktop\brad_resized_by_hristo_cleared_rubbified2.jpg");
        }

        static bool isIn(int x, int y, int width, int height)
        {
            return x >= 0 && x < width && y >= 0 && y < height;
        }

        static Bitmap ClearImage(Bitmap image)
        {
            const int multipleCoefficient = 5;

            Bitmap newBitmap = new Bitmap(image.Width, image.Height);

            for (int i = 0; i < newBitmap.Width; i++)
            {
                for (int j = 0; j < newBitmap.Height; j++)
                {
                    int couterOfNeighbouringPixels = 0;
                    Color color = image.GetPixel(i, j);
                    double sumR = color.R * multipleCoefficient;
                    double sumG = color.G * multipleCoefficient;
                    double sumB = color.B * multipleCoefficient;

                    for (int k = -2; k <= 2; k++)
                    {
                        for (int l = -2; l <= 2; l++)
                        {
                            if (isIn(i + k, j + l, image.Width, image.Height))
                            {
                                couterOfNeighbouringPixels++;
                                var colorOfNeighbouringPixel = image.GetPixel(i + k, j + l);

                                sumR += colorOfNeighbouringPixel.R;
                                sumG += colorOfNeighbouringPixel.G;
                                sumB += colorOfNeighbouringPixel.B;
                            }
                        }
                    }

                    sumR = sumR / (multipleCoefficient + couterOfNeighbouringPixels);
                    sumG = sumG / (multipleCoefficient + couterOfNeighbouringPixels);
                    sumB = sumB / (multipleCoefficient + couterOfNeighbouringPixels);

                    newBitmap.SetPixel(i, j, Color.FromArgb((int)Math.Round(sumR), (int)Math.Round(sumG), (int)Math.Round(sumB)));

                    //double minDistance = int.MaxValue;

                    //Color colorCurrent = bitmap.GetPixel(i, j);

                    //foreach (var point in rubikColors)
                    //{
                    //    double currentDistance = getDistanceBetweenTwoPoints(colorCurrent.R, colorCurrent.G, colorCurrent.B, point[0], point[1], point[2]);
                    //    if (currentDistance < minDistance)
                    //    {
                    //        minDistance = currentDistance;
                    //        Color newColor = Color.FromArgb(point[0], point[1], point[2]);
                    //        bitmap.SetPixel(i, j, newColor);
                    //    }
                    //}


                }
            }

            return newBitmap;
        }

        public const string filePath = @"wwwroot\utilities\input.jpg", resizedOutput = @"wwwroot\utilities\resized.jpg", pixilised = @"wwwroot\utilities\pixilised.jpg";

        private static Image image;

        public static void Scale(int width, int height)
        {
            using (image = Image.FromFile(filePath))
            {
                Bitmap newImage = new Bitmap(width, height);
                using (Graphics g = Graphics.FromImage(newImage))
                {

                    g.DrawImage(image, 0, 0, width, height);
                    newImage.Save(resizedOutput);

                }
            }


        }

    }

}