import logging
import logging.handlers

# 로그 생성
logger = logging.getLogger()
# logger = logging.getLogger()

# 로그의 출력 기준 설정
logger.setLevel(logging.INFO)

# log 출력 형식
# formatter = logging.Formatter('%(asctime)s - %(message)s', '%Y-%m-%d %H:%M:%S')
formatter = logging.Formatter('%(asctime)s - %(message)s')

# log 출력
# stream_handler = logging.StreamHandler()
# stream_handler.setFormatter(formatter)
# logger.addHandler(stream_handler)

# log를 파일에 출력
log_max_size = 10 * 1024 * 1024
# log_max_size = 10 *  1024 : 20sec
log_file_count = 12
# file_handler = logging.handlers.RotatingFileHandler(filename='c:/temp/log.log', maxBytes=log_max_size,
#                                                    backupCount=log_file_count)
file_handler = logging.handlers.RotatingFileHandler(filename='/home/pi/log/log.log', maxBytes=log_max_size,
                                                   backupCount=log_file_count)
# file_handler = logging.FileHandler('/home/pi/log/rxMsg.log')
file_handler.setFormatter(formatter)
logger.addHandler(file_handler)


def printLog(data):
    logger.info(data)   


        