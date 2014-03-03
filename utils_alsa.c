// utils_alsa.c
//
// Copyright 2011 Linaro Limited
//
// kurt.taylor@linaro.org
//

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>


int get_sample(char *device_name, int num_samples, int sample_rate, unsigned short sine_input[])
{
   int rc;
   snd_pcm_hw_params_t *parms;
   snd_pcm_t *handle;

   /* Open the device */
   if ((rc = snd_pcm_open (&handle, device_name, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
      fprintf (stderr, "%s: cannot open pcm device: %s \n", snd_strerror (rc), device_name);
      return (-1);
   }  
   //fprintf (stdout, "Device Opened %s \n", device_name);

   /* Configure the device */
   if ((rc = snd_pcm_hw_params_malloc (&parms)) < 0) {
      fprintf (stderr, "%s: cannot malloc parms struct\n", snd_strerror (rc));
      return (-1);
   }
   if ((rc = snd_pcm_hw_params_any (handle, parms)) < 0) {
      fprintf (stderr, "%s: cannot set default parms\n", snd_strerror (rc));
      return (-1);
   }
   if ((rc = snd_pcm_hw_params_set_access (handle, parms, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
      fprintf (stderr, "%s: cannot set interleaved access\n", snd_strerror (rc));
      return (-1);
   }
   if ((rc = snd_pcm_hw_params_set_format (handle, parms, SND_PCM_FORMAT_U8)) < 0) {
      fprintf (stderr, "%s: cannot set format\n", snd_strerror (rc));
      return (-1);
   }
   if ((rc = snd_pcm_hw_params_set_channels (handle, parms, 2)) < 0) {
      fprintf (stderr, "%s: cannot set channels\n", snd_strerror (rc));
      return (-1);
   }
   if ((rc = snd_pcm_hw_params_set_rate_near (handle, parms, &sample_rate, 0)) < 0) {
      fprintf (stderr, "%s: cannot set rate\n", snd_strerror (rc));
      return (-1);
   }
   if ((rc = snd_pcm_hw_params (handle, parms)) < 0) {
      fprintf (stderr, "%s: cannot set parms\n", snd_strerror (rc));
      return (-1);
   }
   snd_pcm_hw_params_free (parms);

   /* Get samples of the sine wave */
   if ((rc = snd_pcm_readi (handle, sine_input, num_samples)) != num_samples) {
      fprintf (stderr, "%s: read failed\n", snd_strerror (rc));
      return (-1);
   }
   snd_pcm_close (handle);

   return 0;
}
