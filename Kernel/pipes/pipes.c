#include "pipes.h"

size_t open_pipes = 0;
Pipe pipes[MAX_PIPES] = {};

int pipe(int pipefd[2]) {
  if (open_pipes >= MAX_PIPES) {
    return -1;
  }

  size_t pipe_id = 0;
  while (pipe_id < MAX_PIPES && pipes[pipe_id].data) {
    pipe_id++;
  }

  int read_end = open_file(pipe_id * 2, O_PIPE);
  if (read_end < 0) {
    return -1;
  }

  int write_end = open_file(pipe_id * 2 + 1, O_PIPE);
  if (write_end < 0) {
    // _shouldn't_ fail
    close(read_end);
    return -1;
  }

  void *data = malloc(PIPE_SIZE);

  if (!data) {
    // _shouldn't_ fail
    close(read_end);
    close(write_end);
    return -1;
  }

  pipes[pipe_id] = (Pipe){
      .read_ends = 1,
      .write_ends = 1,
      .data = data,
      .read = data,
      .write = data,
  };

  if (sem_init(&pipes[pipe_id].read_sem, 1, 0)) {
    // _shouldn't_ fail
    free(data);
    close(read_end);
    close(write_end);
    return -1;
  }

  if (sem_init(&pipes[pipe_id].write_sem, 1, 1)) {
    // _shouldn't_ fail
    sem_destroy(&pipes[pipe_id].read_sem);
    free(data);
    close(read_end);
    close(write_end);
    return -1;
  }

  open_pipes++;

  pipefd[0] = read_end;
  pipefd[1] = write_end;

  return 0;
}

void add_pipe_end(int file) {
  if (file & 1) {
    pipes[((file ^ O_PIPE) - 1) / 2].write_ends++;
  } else {
    pipes[(file ^ O_PIPE) / 2].read_ends++;
  }
}

int close_pipe(int file) {
  int fd = (file ^ O_PIPE) / 2;
  Pipe *pipe = pipes + fd;

  if (file & 1) {
    if (!--pipe->write_ends) {
      sem_destroy(&pipe->write_sem);

      for (size_t i = 0; i < pipe->read_ends; i++) {
        sem_post(&pipe->read_sem);
      }
    }
  } else {
    if (!--pipe->read_ends) {
      sem_destroy(&pipe->read_sem);
    }
  }

  if (!pipe->read_ends && !pipe->write_ends) {
    free(pipe->data);
    pipe->data = NULL;
    open_pipes--;
  }

  return 0;
}

int read_pipe(int file, void *buf, size_t count) {
  if (file & 1) {
    return -1;
  }

  if (!count) {
    return 0;
  }

  Pipe *pipe = pipes + (file ^ O_PIPE) / 2;

  if (pipe->write_ends && sem_wait(&pipe->read_sem)) {
    return -1;
  }

  size_t avail = pipe->write - pipe->read;
  if (!avail) {
    return 0;
  }

  size_t read = avail < count ? avail : count;

  memcpy(buf, pipe->read, read);

  pipe->read += read;

  if (pipe->read == pipe->write) {
    if (pipe->write == pipe->data + PIPE_SIZE) {
      sem_post(&pipe->write_sem);
    }

    pipe->read = pipe->data;
    pipe->write = pipe->data;
  } else {
    sem_post(&pipe->read_sem);
  }

  return read;
}

int write_pipe(int file, const void *buf, size_t count) {
  if (!(file & 1)) {
    return -1;
  }

  if (!count) {
    return 0;
  }

  Pipe *pipe = pipes + ((file ^ O_PIPE) - 1) / 2;

  if (!pipe->read_ends) {
    return -1;
  }

  if (sem_wait(&pipe->write_sem)) {
    return -1;
  }

  size_t written = pipe->write + count >= pipe->data + PIPE_SIZE
                       ? pipe->data + PIPE_SIZE - pipe->write
                       : count;

  memcpy(pipe->write, buf, written);

  if (pipe->write == pipe->data) {
    sem_post(&pipe->read_sem);
  }

  pipe->write += written;

  if (pipe->write != pipe->data + PIPE_SIZE) {
    sem_post(&pipe->write_sem);
  }

  return written;
}

bool pipe_empty(int file) {
  if (file & 1) {
    return false;
  }

  Pipe pipe = pipes[(file ^ O_PIPE) / 2];

  // If the pipe doesn't have any write end,
  // it must return 0 without blocking when read
  if (!pipe.write_ends) {
    return false;
  }

  return pipe.read == pipe.write;
}

bool pipe_full(int file) {
  if (!(file & 1)) {
    return false;
  }

  Pipe pipe = pipes[(file ^ O_PIPE) / 2];

  return pipe.write == pipe.data + PIPE_SIZE;
}
