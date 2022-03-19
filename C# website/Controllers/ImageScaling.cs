using System.Drawing;
using LazZiya.ImageResize;

namespace C__website{

    public static class ImageScaling{

        public static void Scale(Image image, int width, int height, string outputPath){

            Bitmap newImage = new Bitmap(width, height);
            using(Graphics g = Graphics.FromImage(newImage)){

                g.DrawImage(image, 0, 0, width, height);
                newImage.Save(outputPath);

            }

        }

        public static void ScaleWithAR(Image image, int width, int height, string outputPath){

            Image resized = ImageResize.Scale(image, width, height);
            resized.SaveAs(outputPath);

        }

    }

}