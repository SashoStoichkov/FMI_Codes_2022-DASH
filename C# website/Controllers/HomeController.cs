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

        [HttpPost]
        [ActionName("NewPost")]
        public async Task<IActionResult> NewPostProcessingData(List<IFormFile> files, string description)
        {
            using (var stream = new MemoryStream())
            {
                await files[0].CopyToAsync(stream);

                stream.Seek(0, SeekOrigin.Begin);

                //this.usersPostsService.AddPostToUser(this.User.FindFirstValue(ClaimTypes.NameIdentifier)
                //    , stream.ToArray(), description);
            }


            return this.Redirect("/");

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
