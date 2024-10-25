// MainActivity.kt
package com.example.p1_port

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import com.patrykandpatrick.vico.compose.cartesian.CartesianChartHost
import com.patrykandpatrick.vico.compose.cartesian.axis.rememberAxisLabelComponent
import com.patrykandpatrick.vico.compose.cartesian.axis.rememberBottom
import com.patrykandpatrick.vico.compose.cartesian.axis.rememberStart
import com.patrykandpatrick.vico.compose.cartesian.cartesianLayerPadding
import com.patrykandpatrick.vico.compose.cartesian.layer.rememberLine
import com.patrykandpatrick.vico.compose.cartesian.layer.rememberLineCartesianLayer
import com.patrykandpatrick.vico.compose.cartesian.rememberCartesianChart
import com.patrykandpatrick.vico.compose.cartesian.rememberVicoZoomState
import com.patrykandpatrick.vico.compose.common.component.rememberShapeComponent
import com.patrykandpatrick.vico.compose.common.dimensions
import com.patrykandpatrick.vico.compose.common.fill
import com.patrykandpatrick.vico.compose.common.shader.verticalGradient
import com.patrykandpatrick.vico.compose.common.shape.rounded
import com.patrykandpatrick.vico.core.cartesian.axis.HorizontalAxis
import com.patrykandpatrick.vico.core.cartesian.axis.VerticalAxis
import com.patrykandpatrick.vico.core.cartesian.data.CartesianChartModelProducer
import com.patrykandpatrick.vico.core.cartesian.data.lineSeries
import com.patrykandpatrick.vico.core.cartesian.layer.LineCartesianLayer
import com.patrykandpatrick.vico.core.common.shader.DynamicShader
import com.patrykandpatrick.vico.core.common.shape.CorneredShape
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import kotlin.random.Random

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            Chart1()
        }
    }
}
@Composable
private fun rememberStartAxisLabel() =
    rememberAxisLabelComponent(
        color = Color.Black,
        margins = dimensions(4.dp),
        padding = dimensions(8.dp, 2.dp),
        background = rememberShapeComponent(Color(0xfffab94d), CorneredShape.rounded(4.dp)),
    )

@Composable
fun Chart1(modifier: Modifier = Modifier.fillMaxSize()) {
    val modelProducer = remember { CartesianChartModelProducer() }
    val xValues = (1..100).toList() // Updated to match Chart9's `x`

    LaunchedEffect(Unit) {
        withContext(Dispatchers.Default) {
            modelProducer.runTransaction {
                lineSeries { series(xValues, xValues.map { Random.nextFloat() * 30 - 10 }) } // y-values between -10 and 20
            }
        }
    }


    val colors = listOf(Color(0xFFFF0000), Color(0xFF00FF00), Color(0xFFFFFF00)) // Red, Green, and Blue

    CartesianChartHost(
        chart =
        rememberCartesianChart(
            rememberLineCartesianLayer(
                LineCartesianLayer.LineProvider.series(
                    LineCartesianLayer.rememberLine(
                        fill = LineCartesianLayer.LineFill.double(
                            fill(colors[0]),
                            //fill(colors[Random.nextInt(3)])
                            fill(colors[1])
                        ),
                       areaFill = LineCartesianLayer.AreaFill.double(
                            topFill = fill(
                                DynamicShader.verticalGradient(
                                    arrayOf(colors[0], Color.Transparent)
                                )
                            ),
                            bottomFill = fill(
                                DynamicShader.verticalGradient(
                                    arrayOf(Color.Transparent, colors[1])
                                )
                            )
                        )
                    )
                )
            ),
            startAxis = VerticalAxis.rememberStart(
                label = rememberStartAxisLabel(),
                horizontalLabelPosition = VerticalAxis.HorizontalLabelPosition.Inside,
            ),
            bottomAxis = HorizontalAxis.rememberBottom(
                itemPlacer = HorizontalAxis.ItemPlacer.segmented()
            ),
            layerPadding = cartesianLayerPadding(scalableStart = 16.dp, scalableEnd = 16.dp),
        ),
        modelProducer = modelProducer,
        modifier = modifier,
        zoomState = rememberVicoZoomState(zoomEnabled = false),
    )
}
private val chartColors = listOf(Color(0xffb983ff), Color(0xff91b1fd), Color(0xff8fdaff))