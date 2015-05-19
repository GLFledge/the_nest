/**
 * 
 */
(function(){
	
	var app = angular.module('Sensor',[]); 
	
	app.controller('SensorController',
			['$scope', function($scope){
				$scope.update = function () {
	                $.get('/TempSensorServoWeb?i=' + $scope.tem );
	            }
	            $scope.$watch('tem' {
	                $scope.update();
	            });
	}]);
	
	
})();


