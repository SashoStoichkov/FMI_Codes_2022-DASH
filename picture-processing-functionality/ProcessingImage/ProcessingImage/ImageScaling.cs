using System.Drawing;
using LazZiya.ImageResize;

namespace ProcessingImage
{

    public class ImageScaling
    {

        public const string filePath = @"wwwroot\utilities\input.jpg", resizedOutput = @"wwwroot\utilities\resized.jpg", pixilised = @"wwwroot\utilities\pixilised.jpg";

        private Image image;

        public void Scale(int width, int height)
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
        public void ScaleWithAR(int width, int height)
        {

            Image resized = ImageResize.Scale(image, width, height);
            resized.SaveAs(resizedOutput);

        }

    }

}