# File2Image
A program I wrote to convert a given file to a series of images
filled with pixels which represent the data from that file.

It can also decode the image(s) back to it's original file.

---
<br>

## Objective  
This projects goal is to generate an image or video from a file. The data in the created media is represented by RGB pixels in a 16x9 ratio. You can convert and store the information of the files in a portible universal format.

---
<br>

## Sources
YouTube

[ChatGPT](https://chat.openai.com)

---  
<br>

## Installation
##### Make sure you have the required [dependencies](#dependencies) before proceeding.
<br>

Download and compile repo:  
``` 
git clone https://github.com/rpeepz/File2Image.git  
cd File2Image  
make
```  
Executable `File2Image` will be created  


---
<br>

## Usage
<br>

`./File2Image [ -e | -d | -c | -m | -t | -r | -v ] [ `<ins>__file__</ins>` ]`

-e : ENCODE a <ins>__file__</ins> to f2i Image file

-d : DECODE a <ins>__f2i Image file__</ins> to its original file

-c : CONVERT a <ins>__f2i Image file__</ins> to f2i Video file

-m : MAP a <ins>__f2i Video file__</ins> to f2i Image file

-t : TRANSFORM a <ins>__file__</ins> to f2i Video file**

-r : REVERT a <ins>__f2i Video file__</ins> to its original file**

-v : Version Info

<br>

##### _** works are still in development_
---
<br>

## Dependencies  
* [libpng][libpng]
* A C language compiler most common would be GCC or Clang.
* Cmake to run the custom Makefile for fast instalation.
* Code was written to run on Unix based systems, it would likely work on a windows machine too but is currently untested. 

---
<br>

## Keep in Touch  
Found a bug? Report it by contacting me on a social below  😃  
or if you feel like you can solve it, make a [pull request]!  
<br>  

You can find me on:
* [Github](https://github.com/rpeepz)  
* [Twitter](https://twitter.com/papagna94) 
* [LinkedIn](https://www.linkedin.com/in/rpapagna-510) 
<!-- * [Medium](https://medium.com/@themichaelbrave)  -->
<!-- * [Home] -->

<br>

---

_This project was attempted for learning purposes only. This repo may not be kept up to date and I cannot guarantee that it will work on your future machines. You are welcome to use and learn from this as you want, but do not implement code that you do not understand. These sources listed above have been invaluable and I advise you to do research on the subject before trying to copy the code you see here._

[libpng]: http://www.libpng.org/pub/png/libpng.html "libpng"
[pull request]: https://github.com/rpeepz/File2Image/pulls "pull away"