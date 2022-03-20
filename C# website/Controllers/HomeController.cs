using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using C__website.Models;
using System.IO;
using Microsoft.AspNetCore.Http;
using ProcessingImage;
using System.Drawing;

namespace C__website.Controllers
{
    public class HomeController : Controller
    {
        private readonly ILogger<HomeController> _logger;

        public HomeController(ILogger<HomeController> logger)
        {
            _logger = logger;
        }

        public IActionResult Index()
        {
            return View();
        }

        [ActionName("Photo")]
        public IActionResult Photo()
        {
            return View();
        }

        [HttpPost]
        [ActionName("GetCube")]
        public IActionResult GetCube(string cubeX, string cubeY)
        {
            Bitmap bitmap = new Bitmap(ImageScaling.pixilised);
            var color = bitmap.GetPixel(int.Parse(cubeX), int.Parse(cubeY));
            return View();
        }

        private static int GCD(int a, int b){

            int Remainder;

            while( b != 0 )
            {
                Remainder = a % b;
                a = b;
                b = Remainder;
            }

            return a;

        }

        private static Tuple<int, int> GetBestRectSlow(int lCubes, int rCubes, double ratio)
        {
            double closestRatio = -rCubes;
            Tuple<int, int> ans = Tuple.Create<int, int>(0, 0);
            
            for (int w = 1; w <= rCubes; w++)
            {
                for (int h = 1; h <= rCubes; h++)
                {
                    if (lCubes <= w * h && w * h <= rCubes)
                    {
                        double curr = (double)w / (double)h;
                        if (Math.Abs(curr - ratio) < Math.Abs(closestRatio - ratio))
                        {
                            ans = Tuple.Create<int, int>(w, h);
                            closestRatio = curr;
                        }
                    }
                }
            }

            return ans;
        }

        private static Tuple<int, int> GetBestRect(int lCubes, int rCubes, double ratio)
        {
            double closestRatio = -rCubes * 2;
            Tuple<int, int> ans = Tuple.Create<int, int>(-1, -1);

            void update(int w, int h)
            {
                double curr = (double)w / (double)h;
                if(lCubes<=w*h && w*h<=rCubes && (Math.Abs(ratio-curr)<Math.Abs(ratio-closestRatio) || ans.Item1==-1))
                {
                    ans = Tuple.Create<int, int>(w, h);
                    closestRatio = curr;
                }
            }

            for (int w = 1; w <= rCubes; w++)
            {
                int minH = (lCubes+w-1) / w;
                int maxH = rCubes / w;

                int mid1 = (int)Math.Floor(w / ratio);
                int mid2 = mid1 + 1;

                update(w, minH);
                update(w, maxH);
                update(w, mid1);
                update(w, mid2);
            }

            return ans;
        }

        private static int RGBToNumber(int[] rgb){

            if(rgb[0] == 255 && rgb[1] == 255 && rgb[2] == 255) return 0;
            if(rgb[0] == 0 && rgb[1] == 255 && rgb[2] == 0) return 1;
            if(rgb[0] == 255 && rgb[1] == 165 && rgb[2] == 0) return 2;
            if(rgb[0] == 255 && rgb[1] == 0 && rgb[2] == 0) return 3;
            if(rgb[0] == 0 && rgb[1] == 0 && rgb[2] == 255) return 4;
            if(rgb[0] == 255 && rgb[1] == 255 && rgb[2] == 0) return 5;

            return -1;

        }

        public static void OutputToAlg(){

            Bitmap image = new Bitmap(ImageScaling.pixilised);
            StreamWriter output = new StreamWriter("output.txt");
            for(int i = 0; i < image.Height; i += 3)
                for(int j = 0; j < image.Width; j += 3){
                    for(int k = 0; k < 3; k++)
                        for(int l = 0; l < 3; l++)
                            output.Write(String.Join(' ', RGBToNumber(new int[3]{image.GetPixel(j + l, i + k).R, image.GetPixel(j + l, i + k).G, image.GetPixel(j + l, i + k).B})) + ' ');

                    output.WriteLine();
                }
            output.Close();
            image.Dispose();

        }

        private static string[] GetMoves(){

            return System.IO.File.ReadAllLines("wwwroot/algorithm/solution.txt");

        }

        [HttpPost]
        [ActionName("NewPost")]
        public async Task<IActionResult> NewPostProcessingData(List<IFormFile> files, int number, AspectRatio ar)
        {
            int width, height;
            
            using (var stream = new MemoryStream())
            {
                System.IO.File.Delete(ImageScaling.filePath);
                System.IO.File.Delete(ImageScaling.resizedOutput);
                System.IO.File.Delete(ImageScaling.pixilised);

                await files[0].CopyToAsync(stream);

                stream.Seek(0, SeekOrigin.Begin);

                System.IO.File.WriteAllBytes(ImageScaling.filePath ,stream.ToArray());
                //this.usersPostsService.AddPostToUser(this.User.FindFirstValue(ClaimTypes.NameIdentifier)
                //    , stream.ToArray(), description);
                
                Image image = null;
                try{

                    image = Image.FromFile(ImageScaling.filePath);

                    if(ar == AspectRatio.Landscape){

                        Tuple<int, int> sides = GetBestRect((int)(number * 0.8), number, 16.0 / 9.0);
                        width = sides.Item1;
                        height = sides.Item2;

                    }
                    else if(ar == AspectRatio.Portrait){

                        Tuple<int, int> sides = GetBestRect((int)(number * 0.8), number, 9.0 / 16.0);
                        width = sides.Item1;
                        height = sides.Item2;


                    }
                    else if(ar == AspectRatio.Square){

                        Tuple<int, int> sides = GetBestRect((int)(number * 0.8), number, 1);
                        width = sides.Item1;
                        height = sides.Item2;

                    }
                    else{

                        int gcd = GCD(image.Width, image.Height);
                        width = image.Width / gcd;
                        height = image.Height / gcd;

                        Tuple<int, int> sides = GetBestRect((int)(number * 0.8), number, width * 1.0 / height);
                        width = sides.Item1;
                        height = sides.Item2;

                    }

                }
                finally{

                    image.Dispose();

                }
                
                ProcessingImage.ImageProcess.ProcessImage(width * 3, height * 3);
            }

            Models.Arguments args = new Models.Arguments();
            args.width = width;
            args.height = height;

            OutputToAlg();

            args.moves = GetMoves();

            return View("Photo", args);

        }

        public IActionResult Privacy()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
