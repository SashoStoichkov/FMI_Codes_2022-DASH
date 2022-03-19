using System.Drawing;
using LazZiya.ImageResize;

namespace ProcessingImage
{

    public static class ImageScaling
    {

        public const string filePath = @"..\utilities\input.jpg", resizedOutput = @"..\utilities\resized.jpg", pixilised = @"..\utilities\pixilised.jpg";

        private static Image image = Image.FromFile(filePath);

        public static void Scale(int width, int height)
        {

            Bitmap newImage = new Bitmap(width, height);
            using (Graphics g = Graphics.FromImage(newImage))
            {

                g.DrawImage(image, 0, 0, width, height);
                newImage.Save(resizedOutput);

            }

        }

        public static void ScaleWithAR(int width, int height)
        {

            Image resized = ImageResize.Scale(image, width, height);
            resized.SaveAs(resizedOutput);

        }

    }

}