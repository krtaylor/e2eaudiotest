// testfreq.c
//
// Copyright 2011 Linaro Limited
//
// kurt.taylor@linaro.org
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <alsa/asoundlib.h>
#include <complex.h>
#include <fftw3.h>

int get_sample (char *device_name, int num_samples, int sample_rate, unsigned short sine_input[]);

int main (int argc, char *argv[])
{
   int i, rc;
   int sample_rate = 48000;
   int num_samples = 4 * 16384;

   char *device_name = "default";  /* pcm.default */
   
   unsigned short sine_input[num_samples];
   double f_input[num_samples];
   double f_output[num_samples];
   fftw_plan plan;
   
   double max = 0;
   double absolute_val, correction;
   int freq;
   
   if (argc > 1) {
      device_name = argv[1];
   }

   if ((rc = get_sample (device_name, num_samples, sample_rate, sine_input)) < 0) {
      fprintf (stderr, "%s: cannot get sample\n", snd_strerror (rc));
      exit (1);
   }

   /* Do DFT */
   plan = fftw_plan_r2r_1d (num_samples, f_input, f_output, FFTW_R2HC, FFTW_ESTIMATE);
   for (i = 0; i < num_samples; i++){
      f_input[i] = (sine_input[i] - 128) * (.54 -.46 * cos(2.0 * M_PI * i / (num_samples - 1)));
   }
   fftw_execute (plan);
   //fprintf (stdout, "FFTW plan executed\n");

   /* Figure out primary frequency (largest) */
   correction = (double)sample_rate / (double)num_samples;
   for (i = 2; i < num_samples / 16; i++) {
      absolute_val = f_output[i] * f_output[i] + f_output[num_samples - i] * f_output[num_samples - i];
      if (absolute_val > max) {
         max = absolute_val;
         freq = i * correction;
	 //fprintf (stdout, "Found max freq: %iHz\n", freq);
      }
   }
   fftw_destroy_plan (plan);

   /* Report results */
   fprintf(stdout, "Freq: %iHz\n", freq);
   if ((freq >= 435) && (freq <= 445)) {
      fprintf(stdout, "e2eaudiotest has detected a sinewave at 440 Hz\n");
      exit (0);
   }

   fprintf(stdout, "e2eaudiotest failed to detect a sinewave at 440 Hz\n");
   exit (1);
}

