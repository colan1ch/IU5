# ===============================================
# Часть 3: Прогнозирование временных рядов в R
# ===============================================

# 1. Подключаем необходимые библиотеки
library(forecast)   # основная библиотека для прогнозирования
library(ggplot2)    # для красивой визуализации (autoplot использует ggplot2)

# 2. Загружаем встроенный датасет AirPassengers
# Это месячные данные о количестве авиапассажиров (1949–1960)
data("AirPassengers")

# Для удобства переименуем в X
X <- AirPassengers

# Посмотрим, что у нас есть
print(X)
class(X)        
start(X)       
end(X)          
frequency(X)    

# 3. Разделяем данные на обучающую и тестовую выборки в пропорции 80/20
n <- length(X)
train_size <- round(0.8 * n)      

train <- window(X, end = c(1949 + (train_size - 1) %/% 12, (train_size - 1) %% 12 + 1))
test  <- window(X, start = c(1949 + train_size %/% 12, train_size %% 12 + 1))


length(train)   
length(test)    

# 4. Метод 1: Naive (наивный) прогноз
naive_forecast <- naive(train, h = length(test))
autoplot(naive_forecast) +
  autolayer(test, series = "Тестовые данные", color = "red") +
  labs(title = "Naive forecast", y = "Пассажиры", x = "Год")

accuracy(naive_forecast, test)

# 5. Метод 2: Экспоненциальное сглаживание (ETS)
ets_model <- ets(train)
ets_forecast <- forecast(ets_model, h = length(test))

autoplot(ets_forecast) +
  autolayer(test, series = "Тестовые данные", color = "red") +
  labs(title = "Exponential Smoothing (ETS)", y = "Пассажиры", x = "Год")

accuracy(ets_forecast, test)

# 6. Метод 3: Автоматический ARIMA (самый мощный)
arima_model <- auto.arima(train, seasonal = TRUE)
arima_forecast <- forecast(arima_model, h = length(test))

autoplot(arima_forecast) +
  autolayer(test, series = "Тестовые данные", color = "red") +
  labs(title = "Auto ARIMA forecast", y = "Пассажиры", x = "Год")

accuracy(arima_forecast, test)

# 7. Сравнение точности всех трёх методов на тестовой выборке
cat("=== Сравнение точности на тестовой выборке ===\n")
print(accuracy(naive_forecast, test))
print(accuracy(ets_forecast, test))
print(accuracy(arima_forecast, test))

# Обычно ARIMA даёт наименьшие MAE, RMSE, MAPE