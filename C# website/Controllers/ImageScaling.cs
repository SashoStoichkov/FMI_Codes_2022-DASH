using System.Drawing;
using LazZiya.ImageResize;

namespace C__website{

    public static class ImageScaling{

        private const string filePath = "Images/input.jpg", output = "Images/output.jpg";
        private static Image image = Image.FromFile(filePath);

        public static void Scale(int width, int height){

            Bitmap newImage = new Bitmap(width, height);
            using(Graphics g = Graphics.FromImage(newImage)){

                g.DrawImage(image, 0, 0, width, height);
                newImage.Save(output);

            }

        }

        public static void ScaleWithAR(int width, int height){

            Image resized = ImageResize.Scale(image, width, height);
            resized.SaveAs(output);

        }

    }

}