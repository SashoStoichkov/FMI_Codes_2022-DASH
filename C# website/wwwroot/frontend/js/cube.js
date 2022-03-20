// add n cubes to the html page
function addCubes(n) {
  var html = "";
  for (var i = 0; i < n; i++) {
    html += '<div class="square" id="square_' + i + '"></div>';
  }
  $("#grid").html(html);
}

function getRandomColor() {
  var colors = [
    "rgb(255,255,255)",
    "rgb(255,0,0)",
    "rgb(0,255,0)",
    "rgb(0,0,255)",
    "rgb(255,255,0)",
    "rgb(255,165,0)",
  ];
  return colors[Math.floor(Math.random() * colors.length)];
}

function randomColors() {
  // for each square set the color
  $(".square").each(function (i) {
    $(this).css("background-color", getRandomColor());
  });
}

function setSize(width, height) {
  // get the size from the input
  // var width = $("#width").val();
  // var height = $("#height").val();

  // set the size of the grid
  $("#grid").css("grid-template-columns", "repeat(" + width * 3 + ", 0fr)");
  $("#grid").css("grid-template-rows", "repeat(" + height * 3 + ", 0fr)");

  console.log("Hello");
  // add the cubes
  addCubes(width * height * 9);

  $("#grid").css("width", width * 33 - 1 + "px");
  $("#grid").css("height", height * 33 - 1 + "px");

  $("#grid").css("background-color", "black");

  getColorsFromImage();

  // unhide the download button
  if (width > 0 && height > 0) {
    $("#download").css("display", "block");
  }
}

function getColorsFromImage() {
  // get pixel data from image file in /images
  var img = new Image();
  img.src = "../utilities/pixilised.jpg";
  img.onload = function () {
    var canvas = document.createElement("canvas");
    canvas.width = img.width;
    canvas.height = img.height;
    var ctx = canvas.getContext("2d");
    ctx.drawImage(img, 0, 0);
    var imgData = ctx.getImageData(0, 0, img.width, img.height);
    var data = imgData.data;
    var colors = [];
    for (var i = 0; i < data.length; i += 4) {
      var r = data[i];
      var g = data[i + 1];
      var b = data[i + 2];
      var color = "rgb(" + r + "," + g + "," + b + ")";
      colors.push(color);
    }

    // for each square set the color
    $(".square").each(function (i) {
      $(this).css("background-color", colors[i]);
    });
  };
}

function downloadImage() {
  // get the image
  var canvas = document.getElementById("grid");
  html2canvas(canvas).then(function (canvas) {
    var img = canvas.toDataURL("image/png");
    var link = document.createElement("a");
    link.download = $("#filename").val() + ".png";
    link.href = img;
    link.click();
  });
}

function showInstructions() {
  var x = document.getElementById("instructions");
  if (x.style.display === "none") {
    x.style.display = "block";
  } else {
    x.style.display = "none";
  }
}
