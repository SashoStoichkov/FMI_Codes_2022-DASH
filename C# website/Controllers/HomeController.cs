﻿using System;
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
using System.Math;

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
                
                Image image = Image.FromFile(ImageScaling.filePath);

                if(ar == AspectRatio.Landscape){

                    height = Math.sqrt(number / 2);
                    width = height * 2;

                }
                else if(ar == AspectRatio.Portrait){

                    width = Math.sqrt(number / 2);
                    height = 2 * width;

                }
                else if(ar == AspectRatio.Square){

                    width = Math.sqrt(number);
                    height = width;

                }
                else{

                    int gcd = GCD(image.Width, image.Height);
                    width = image.Width / gcd;
                    height = image.Height / gcd;

                }

                ProcessingImage.ImageProcess.ProcessImage(90,90);
            }

            return this.Redirect("Photo");

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
