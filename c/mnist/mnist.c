#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include "mnist.h"

struct mnist_dataset_t;

typedef struct mnist_dataset_t * mnist_dataset_handle;
typedef const struct mnist_dataset_t * const_mnist_dataset_handle;

struct mnist_image_t;
typedef struct mnist_image_t * mnist_image_handle;
typedef const struct mnist_image_t * const_mnist_image_handle;

typedef struct mnist_dataset_t{
	int *bslabel;
	int *bsimg;
	mnist_image_handle first;
}mnist_dataset_t;

typedef struct mnist_image_t{
	unsigned char *image;
	char *label;
	struct mnist_image_t *next;
	struct mnist_image_t *previous;
}mnist_image_t;

mnist_dataset_handle mnist_open (const char * name){
	int area;
	mnist_dataset_handle dh=(mnist_dataset_handle)malloc(sizeof(mnist_dataset_t));
	int *b1=(int*)malloc(2*sizeof(int));int *b2=(int*)malloc(4*sizeof(int));
	mnist_image_handle ih=(mnist_image_handle)malloc(sizeof(mnist_image_t));
	dh->bslabel=b1;dh->bsimg=b2;dh->first=ih;
	mnist_image_handle head,temp=dh->first;//first,head,temp都先指向同一个image_t
	FILE *lab;FILE *img;
	char fn[256];char fnn[256];
	snprintf(fn,sizeof(fn),"%s-labels.idx1-ubyte",name);
	snprintf(fnn,sizeof(fnn),"%s-images.idx3-ubyte",name);
	lab=fopen(fn,"rb");
	img=fopen(fnn,"rb");
	if(lab!=NULL && img!=NULL){
		fseek(lab, 0, SEEK_SET);fseek(img, 0, SEEK_SET);
		fread(b1,sizeof(int),2,lab);fread(b2,sizeof(int),4,img);
		if(ntohl(*b1)!= 2049 || ntohl(*b2)!= 2051){return MNIST_DATASET_INVALID;}
		else{
			fseek(lab, 8, SEEK_SET );fseek(img, 16, SEEK_SET );
			area=ntohl(*(b2+2))*ntohl(*(b2+3));
			char *lb=(char*)malloc(sizeof(char));
			unsigned char *im=(unsigned char*)malloc(area*sizeof(char));//b2+2，b2+3对应行列数
			ih->label=lb;ih->image=im;ih->previous=NULL;//the info in first image_t
			fread(lb,sizeof(char),1,lab);fread(im,sizeof(char),area,img);//first node
			for(int i=0;i<ntohl(*(b1+1))-1;i++){//b1+1是label or 图片数
				head=(mnist_image_handle)malloc(sizeof(mnist_image_t));
				lb=(char*)malloc(sizeof(char));im=(unsigned char*)malloc(area*sizeof(char));
				head->label=lb;head->image=im;
				fread(lb,sizeof(char),1,lab);fread(im,sizeof(char),area,img);
				head->previous=temp;
				temp->next=head;
				temp=head;
				if(i==ntohl(*(b1+1))-2){head->next=NULL;}
			}
			*b1=ntohl(*b1);*(b1+1)=ntohl(*(b2+1));
			*b2=ntohl(*b2);*(b2+1)=ntohl(*(b2+1));
			*(b2+2)=ntohl(*(b2+2));*(b2+3)=ntohl(*(b2+3));

			// printf("%d",dh->first->next->next->label[0]);
			// for(int i=1;i<=28;i++){
			// 	printf("\n");
			// 	for(int j=1;j<=28;j++){	
			// 		printf("%02X",dh->first->next->next->image[(i-1)*28+j-1]);
			// 	}
			// }
			
		}
		fclose(lab);fclose(img);
		return dh;
	}
	else{fclose(lab);fclose(img);return MNIST_DATASET_INVALID;}
}

mnist_dataset_handle mnist_open_sample (const char * name, unsigned int k){
	int area;
	mnist_dataset_handle dh=(mnist_dataset_handle)malloc(sizeof(mnist_dataset_t));
	int *b1=(int*)malloc(2*sizeof(int));int *b2=(int*)malloc(4*sizeof(int));
	mnist_image_handle ih=(mnist_image_handle)malloc(sizeof(mnist_image_t));
	dh->bslabel=b1;dh->bsimg=b2;dh->first=ih;
	mnist_image_handle head,temp=dh->first;//first,head,temp都先指向同一个image_t
	FILE *lab;FILE *img;
	char fn[256];char fnn[256];
	snprintf(fn,sizeof(fn),"%s-labels.idx1-ubyte",name);
	snprintf(fnn,sizeof(fnn),"%s-images.idx3-ubyte",name);
	lab=fopen(fn,"rb");
	img=fopen(fnn,"rb");
	if(lab!=NULL && img!=NULL){
		fseek(lab, 0, SEEK_SET);fseek(img, 0, SEEK_SET);
		fread(b1,sizeof(int),2,lab);fread(b2,sizeof(int),4,img);
		if(ntohl(*b1)!= 2049 || ntohl(*b2)!= 2051){return MNIST_DATASET_INVALID;}
		else{
			fseek(lab, 8, SEEK_SET );fseek(img, 16, SEEK_SET );
			area=ntohl(*(b2+2))*ntohl(*(b2+3));
			char *lb=(char*)malloc(sizeof(char));
			unsigned char *im=(unsigned char*)malloc(area*sizeof(char));//b2+2，b2+3对应行列数
			ih->label=lb;ih->image=im;ih->previous=NULL;//the info in first image_t
			fread(lb,sizeof(char),1,lab);fread(im,sizeof(char),area,img);//first node
			if(k<=ntohl(*(b1+1))){
				for(int i=0;i<k-1;i++){//建立长度为k的链表
					head=(mnist_image_handle)malloc(sizeof(mnist_image_t));
					lb=(char*)malloc(sizeof(char));im=(unsigned char*)malloc(area*sizeof(char));
					head->label=lb;head->image=im;
					fread(lb,sizeof(char),1,lab);fread(im,sizeof(char),area,img);
					head->previous=temp;
					temp->next=head;
					temp=head;
					if(i==k-2){head->next=NULL;}
				}
				mnist_image_handle search;
				for(int i=k;i<ntohl(*(b1+1));++i){
					int j=rand()%(i+1);
					if(j<k){
						//printf("%d\t%d\n",j,i-k);
						search=ih;
						for(int count=0;count<j;++count){
							search=search->next;
						}
						fseek(lab, 8+i, SEEK_SET );fseek(img, 16+i*area, SEEK_SET );
						fread(search->label,sizeof(char),1,lab);
						fread(search->image,sizeof(char),area,img);	
					}
				}
				*b1=ntohl(*b1);*(b1+1)=k;
				*b2=ntohl(*b2);*(b2+1)=k;*(b2+2)=ntohl(*(b2+2));*(b2+3)=ntohl(*(b2+3));
			}
			else{fclose(lab);fclose(img);return MNIST_DATASET_INVALID;}
		}
		fclose(lab);fclose(img);
		return dh;
	}
	else{fclose(lab);fclose(img);return MNIST_DATASET_INVALID;}
}

mnist_dataset_handle mnist_create(unsigned int x, unsigned int y){
	mnist_dataset_handle dh=(mnist_dataset_handle)malloc(sizeof(mnist_dataset_t));
	int *b1=(int*)malloc(2*sizeof(int));int *b2=(int*)malloc(4*sizeof(int));
	mnist_image_handle ih=(mnist_image_handle)malloc(sizeof(mnist_image_t));
	dh->bslabel=b1;dh->bsimg=b2;dh->first=ih;
	*b1=2049;*(b1+1)=0;
	*b2=2051;*(b2+1)=0;*(b2+2)=x;*(b2+3)=y;
	return dh;
}

void mnist_free (mnist_dataset_handle handle){
	mnist_image_handle temp;
	while(handle->first !=NULL){
		temp=handle->first;
		handle->first=handle->first->next;
		free(temp->image);free(temp->label);
		free(temp);
	}
	free(handle->bslabel);free(handle->bsimg);
}

int mnist_image_count (const_mnist_dataset_handle handle){
	if(handle==MNIST_DATASET_INVALID){return -1;}
	else{
		return *(handle->bslabel+1);
	}
}

void mnist_image_size (const_mnist_dataset_handle handle,
						unsigned int * x, unsigned int * y){
	if(handle==MNIST_DATASET_INVALID){*x=0;*y=0;}
	else{
		*x=*(handle->bsimg+2);
		*y=*(handle->bsimg+3);
	}
}

mnist_image_handle mnist_image_begin (mnist_dataset_handle handle){
	if(handle==MNIST_DATASET_INVALID || *(handle->bslabel+1)==0){
		return MNIST_IMAGE_INVALID;
	}
	else{return handle->first;}
}
//不能当做string
const unsigned char * mnist_image_data (const_mnist_image_handle h){
	if(h !=MNIST_IMAGE_INVALID){
		return h->image;
	}
	else{return NULL;}
}

mnist_image_handle mnist_image_next (mnist_image_handle h){
	if(h->next != NULL && h !=MNIST_IMAGE_INVALID){
		return h->next;
	}
	else{return MNIST_IMAGE_INVALID;}
}

int mnist_image_label (const_mnist_image_handle h){
	if(h !=MNIST_IMAGE_INVALID){return *(h->label);}
	else{return -1;}
}

int mnist_set_image_label(mnist_image_handle h, 
                                   unsigned int newlabel){
	if(h !=MNIST_IMAGE_INVALID){
		*(h->label)=(char)newlabel;
		return 0;}
	else{return -1;}
}

mnist_image_handle mnist_image_prev (mnist_image_handle h){
	if(h !=MNIST_IMAGE_INVALID && h->previous !=NULL){
		return h->previous;
	}
	else{return MNIST_IMAGE_INVALID;}
}


mnist_image_handle mnist_image_add_after (mnist_dataset_handle dataset,
        mnist_image_handle h,
      const unsigned char * imagedata, unsigned int x, unsigned int y){
	if(dataset != MNIST_DATASET_INVALID){
		mnist_image_handle new;
		if(h==MNIST_IMAGE_INVALID){
			if(x != *(dataset->bsimg+2) || y != *(dataset->bsimg+3)){
				return MNIST_IMAGE_INVALID;}
			else{
				new=(mnist_image_handle)malloc(sizeof(mnist_image_t));
				unsigned char *temp=(unsigned char *)malloc(x*y*sizeof(char));
				char *tt=(char*)malloc(sizeof(char));*tt=0;
				memcpy(temp,imagedata,x*y);
				new->image=temp;new->previous=NULL;new->label=tt;
				if(dataset->first != NULL && dataset->first != MNIST_IMAGE_INVALID){
					new->next=dataset->first;
					(dataset->first)->previous=new;
				}
				else{new->next=NULL;}
			}
			*(dataset->bslabel+1)=*(dataset->bslabel+1)+1;
			*(dataset->bsimg+1)=*(dataset->bsimg+1)+1;
			return new;
		}
		else{
			if(x != *(dataset->bsimg+2) || y != *(dataset->bsimg+3)){
				return MNIST_IMAGE_INVALID;}
			else{
				new=(mnist_image_handle)malloc(sizeof(mnist_image_t));
				unsigned char *temp=(unsigned char *)malloc(x*y*sizeof(char));
				char *tt=(char*)malloc(sizeof(char));*tt=0;
				memcpy(temp,imagedata,x*y);
				new->image=temp;new->label=tt;
				new->previous=h;new->next=h->next;h->next=new;
			}
			*(dataset->bslabel+1)=*(dataset->bslabel+1)+1;
			*(dataset->bsimg+1)=*(dataset->bsimg+1)+1;
			return new;
		}
	}
	else{return MNIST_IMAGE_INVALID;}
}

mnist_image_handle mnist_image_remove(mnist_dataset_handle dataset,
        mnist_image_handle h){
	mnist_image_handle search;
	if(dataset != MNIST_DATASET_INVALID){
		if(h !=MNIST_IMAGE_INVALID){
			search=dataset->first;
			if(search==h){
				if(search->next==NULL || search->next==MNIST_IMAGE_INVALID){
					return MNIST_IMAGE_INVALID;//still need to remove
				}
				else{
					search->next->previous=NULL;
					return search->next;
				}
			}
			else{
				for(int count=0;count<*(dataset->bsimg+1)-1;++count){
					search=search->next;
					if(search==h){
						if(search->next==NULL || search->next==MNIST_IMAGE_INVALID){
							return MNIST_IMAGE_INVALID;
						}
						else{
							search->next->previous=search->previous;
							search->previous->next=search->next;
							return search->next;
						}
					}
				}
				return MNIST_IMAGE_INVALID;//not found
			}
		}
		else{return MNIST_IMAGE_INVALID;}
	}
	else{return MNIST_IMAGE_INVALID;}
}

bool mnist_save(const_mnist_dataset_handle k, const char * filename){
    mnist_dataset_handle h=(mnist_dataset_handle)k;
	char fn[256];char fnn[256];
	snprintf(fn,sizeof(fn),"%s-labels.idx1-ubyte",filename);
	snprintf(fnn,sizeof(fnn),"%s-images.idx3-ubyte",filename);
	FILE *lab=fopen(fn,"w");
	FILE *img=fopen(fnn,"w");
	if(h != MNIST_DATASET_INVALID){
		unsigned int a,b;
		mnist_image_size(h,&a,&b);
		int area=a*b;
		int count=mnist_image_count (h);
		fprintf (lab, "%08x%08x", *(h->bslabel), *(h->bslabel+1));
	    fprintf (img, "%08x%08x%08x%08x", *(h->bsimg), *(h->bsimg+1),
	    	*(h->bsimg+2), *(h->bsimg+3));
	    mnist_image_handle image =mnist_image_begin(h);
	    for (unsigned int i=0; i<count; ++i){
		   const unsigned char *data=mnist_image_data(image);
		   for(int j=0;j<area;++j){
		   	  fprintf (img,"%02x",*(data+j)); 
		   }
	   	   fprintf (lab,"%02x",mnist_image_label(image));
	   	   image=mnist_image_next(image);
	    }
	    fclose(lab);fclose(img);
	    return true;
	}
	else{fclose(lab);fclose(img);return false;}
}

// int main(){
// 	mnist_dataset_handle dh=mnist_open("train");
// 	mnist_image_handle h=mnist_image_begin(dh);
// 	unsigned char *im=h->image;
// 	int a=memcmp(im+178,"^",1);
// 	printf("%02x\n",*(im+178));
// 	if(a==0){
// 		printf("yes\n");
// 	}
	//printf("%d\n",mnist_save(dh,"test"));
	// mnist_image_handle h=mnist_image_begin(dh);
	// printf("%d\n",mnist_image_label(h));
	// const unsigned char *test=mnist_image_data(h);	
	// for(int i=1;i<=28;i++){
	// 	printf("\n");
	// 	for(int j=1;j<=28;j++){
	// 		printf("%02X",test[(i-1)*28+j-1]);
	// 	}
	// }
// 	return 0;
// }

// int main (int argc, char ** args)
// {
//    if (argc != 3)
//    {
//       fprintf (stderr, "Need 2 arguments:\n  - dataset name\n  - number of images to convert. Use 0 for all.");
//       exit(1);
//    }

//    const char * name = args[1];
//    unsigned int count = atoi(args[2]);

//    // Open dataset
//    mnist_dataset_handle h = mnist_open (name);
//    if (!h)
//    {
//       fprintf (stderr, "Couldn't open dataset '%s'!\n", name);
//       return 1;
//    }

//    const unsigned int imagecount = mnist_image_count (h);
//    unsigned int width, height;
//    mnist_image_size (h, &width, &height);

//    printf ("Images=%u, width=%u, height=%u\n", imagecount, width, height);

//    if (!count)
//       count = imagecount;

//    char filename[255];
//    mnist_image_handle img = mnist_image_begin(h);
//    for (unsigned int i=0; i<count; ++i)
//    {
//       if (!img)
//       {
//          fprintf (stderr, "Not enough images in dataset!\n");
//          break;
//       }
// 	  if(i==count-1){
// 	      snprintf (filename, sizeof(filename)-1, "%s-%05u.pgm", name, i);
// 	      // Ensure 0-terminated
// 	      filename[sizeof(filename)-1]=0;

// 	      FILE * f = fopen (filename, "w");
// 	      if (!f)
// 	      {
// 	         perror ("Couldn't write to file: ");
// 	         mnist_free (h);
// 	         return 1;
// 	      }

// 	      printf ("Writing %s...\n", filename);

// 	      fputs ("P2\n", f);
// 	      fprintf (f, "# %s %u/%u\n", name, i, imagecount);
// 	      fprintf (f, "%u %u\n", width, height);
// 	      fputs ("255\n", f);

// 	      const unsigned char * data = mnist_image_data (img);
// 	      for (unsigned int y=0; y<height; ++y)
// 	      {
// 	         for (unsigned int x=0; x<width; ++x)
// 	         {
// 	            unsigned int c = *data;
// 	            fprintf (f, "%u ", c);
// 	            ++data;
// 	         }
// 	         fputs ("\n", f);
// 	      }
// 	      fclose (f);
// 	    }

//       img = mnist_image_next (img);
//    }
//    mnist_free (h);
//    exit(EXIT_SUCCESS);
// }











